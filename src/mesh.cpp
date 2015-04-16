#include "mesh.h"


void mesh::init(ncio data, GLSLShader *shader){
    
    enum    {vertex, normal, color, texture, index};
    
    int i,j;
    float   r = 1.0f; // fix this
    float   lat, lon;
    
    // define the mesh here
    // including vertex position, texture coords, normals and/or colors
    
    // map the geolocated coords into cartesian coords.
    int count = 0;
    int triCount = 0;
    
    float min_value = *min_element(data.field.begin(), data.field.end());
    float max_value = *max_element(data.field.begin(), data.field.end());
    
    cout << " range is " << min_value << " to " << max_value << endl;
    
    float range = max_value - min_value;
    
    cout << "range is " << range << endl;
    
    width = data.nlon;
    height = data.nlat;
    cout << "width = " << width << endl;
    cout << "height = " << height << endl;
    
    count = 0;
    //for (j=0;j<field.nlat;j++) { // latitude
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude
    
            
            // normalized data field value
            float data_value = ( (data.field[ (width-i-1) + (width*j)]- min_value) / range);
            
            // set up vertex array
            // convert spherical coords to cartesian
            lat = data.lat[j] * M_PI/180.0f;
            lon = data.lon[i] * M_PI/180.0f;
            
            data_value = 0.0f;
            vertex_coords.push_back( (r+data_value*0.01) * cos(lat) * cos(lon));   // x
            vertex_coords.push_back( (r+data_value*0.01) * sin(lat));   // y
            vertex_coords.push_back( (r+data_value*0.01) * cos(lat) * sin(lon));              // z
            
            /*
            cout << "point is: "<<field.lon[i]<<" "<<field.lat[j]<<endl;
            cout << "tri["<<triCount<<"].x ="<<vertex_coords[count]<<endl;
            cout << "tri["<<triCount<<"].y ="<<vertex_coords[count+1]<<endl;
            cout << "tri["<<triCount<<"].z ="<<vertex_coords[count+2]<<endl;
            */
            triCount++;
            count+=3;
            
            
            // set up texture coords
            texture_coords.push_back(1.0 - (float)(i)/(float)(width));
            texture_coords.push_back(1.0 - (float)j/(float)(height));
            
            
            // set vertex colors
            // this is a greyscale range
            
            //array[width * row + col]
            //x = data.field[ i + (width*j)] / range;
            //return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
            color_coords.push_back( ( (data.field[ (width-i-1) + (width*j)]- min_value) / range) );
            color_coords.push_back( ( (data.field[ (width-i-1) + (width*j)]- min_value) / range) );
            color_coords.push_back( ( (data.field[ (width-i-1) + (width*j)]- min_value) / range) );
            
            
            //return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
            //float data_value = data.field[ i + (width*j)];
            
            //cout << "data value is " << data.field[ (width-i-1) + (width*j)] << endl;
            //cout << "mapped value is " << ( (data.field[ (width-i-1) + (width*j)]- min_value) / range) << endl;
            //cout << " color = " << (data_value - min_value)/range << endl;
            
            /*
            if(i%2==0){
                color_coords.push_back(1.0f);
                color_coords.push_back(0.0f);
                color_coords.push_back(0.0f);
            }
            else if (i%3==0){
                //2nd green
                color_coords.push_back(0.0f);
                color_coords.push_back(1.0f);
                color_coords.push_back(0.0f);
            }
            else{
                //3rd blue
                color_coords.push_back(0.0f);
                color_coords.push_back(0.0f);
                color_coords.push_back(1.0f);
            }
             */
            //4th white
            //color_coords.push_back(1.0f);
            //color_coords.push_back(1.0f);
            //color_coords.push_back(1.0f);
            
        }
    }
    
    cout << "triCOunt = "<<triCount<<endl;
    
    cout << "done vertex" << endl;
    cout << "done normals" << endl;
    
    /*
    // make the first vertex red
    color_coords.push_back(1.0f);
    color_coords.push_back(0.0f);
    color_coords.push_back(0.0f);
    //2nd green
    color_coords.push_back(0.0f);
    color_coords.push_back(1.0f);
    color_coords.push_back(0.0f);
    //3rd blue
    color_coords.push_back(0.0f);
    color_coords.push_back(0.0f);
    color_coords.push_back(1.0f);
    //4th white
    color_coords.push_back(1.0f);
    color_coords.push_back(1.0f);
    color_coords.push_back(1.0f);
    */
    
    // setup triangle face list - this is done per triangle
    // for each triangle we just store the index into the vertex array
    
    
    /*
    // up triangles
    for (j=1;j<field.nlat-1;j++) {		// latitude
        for (i=0;i<field.nlon-1;i++){	// longitude
            
            indices.push_back( (i) + (j*field.nlat)) ;
            indices.push_back( ( (i) + field.nlat) + (j*field.nlat) );
            indices.push_back(( (i+1) + (j*field.nlat) ) );
            
            //printf("#i = %d, count = %d,up: tri = %d, %d, %d\n",i, count, E->mesh.tri[count].a, E->mesh.tri[count].b, E->mesh.tri[count].c);
        }
        //printf("\n");
    }
    
    
    //printf("\n#down tris\n\n");
    
    // now do the down pointing triangles
    for (j=1;j<field.nlat;j++) {		// latitude
        for (i=1;i<field.nlon;i++){		// longitude
            
            indices.push_back(((i) + (j-1)*(field.nlon)) );
            indices.push_back(((i-1) + field.nlon) + (j-1)*(field.nlon));
            indices.push_back(((i) + field.nlon) + (j-1)*(field.nlon));
            
            //printf("#i = %d count = %d, tri = %d, %d, %d\n",i, count, E->mesh.tri[count].a, E->mesh.tri[count].b, E->mesh.tri[count].c);
            
        }
        //printf("\n");
    }
     */
     
    
    
    // this one seems to work
    // when the lat array is flipped
    
    /*
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(0);
    
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(2);
    */
    
    /*
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);
    */
    
    
    // set up index array
    count = 0;
    for (j=1;j<height;j++) { // latitude
        // first triangle for this strip
        indices.push_back(j*width);
        //cout << "j = "<<j<< " index["<<count<<"] = "<<indices[count]<<endl;count++;
        indices.push_back(j*width - width);
        //cout << "j = "<<j<< " index["<<count<<"] = "<<indices[count]<<endl;count++;
        indices.push_back(j*width+1);
        //cout << "j = "<<j<< " index["<<count<<"] = "<<indices[count]<<endl;count++;
        for (i=1;i<width;i++) { // longitude
            indices.push_back(i+(j-1)*width);
            //cout << "i = "<<i<< " index["<<count<<"] = "<<indices[count]<<endl;count++;
            indices.push_back(i+width+1 + +(j-1)*width);
            //cout << "i = "<<i<< " index["<<count<<"] = "<<indices[count]<<endl;count++;
            
        }
        indices.pop_back();
        count--;
        //cout << endl;
    }
    
    /*
    cout << "done indices" << " nIndexes = " << indices.size() << endl;
    for(i=0;i<indices.size();i++){
        cout << "index["<<i<<"] = "<<indices[i]<<endl;
    }
    */
    
    // create VBO
    //
    // each buffer corresponds to:
    //
    //      buffer[0] = vertices
    //      buffer[1] = normals
    //      buffer[2] = colors
    //      buffer[3] = texture_coords
    //
    // the buffer indexing below uses the enum declared above
    // (enum    {vertex, normal, color, texture};)
    //
    
    // create VBO's
    glGenBuffers(5, buffers);
    
    // create VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    
    // set the VAO pointers to the VBO
    if(shader->vertex_coords_name != ""){
        
        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[vertex]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, vertex_coords.size()*sizeof(GLfloat), &vertex_coords[0], GL_STATIC_DRAW);
        // get the attribute location
        vertex_position_attrib_location = shader->GetAttributeLocation(shader->vertex_coords_name);
        // connect the vertex.xyz data to the "v3Position" attribute of the vertex shader
        glEnableVertexAttribArray(vertex_position_attrib_location);
        glVertexAttribPointer(vertex_position_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
        
        
        
        // now bind the index buffer
        // Index buffer
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }
    
    
    if(shader->normals_name != ""){
        
        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[normal]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, normal_coords.size()*sizeof(GLfloat), &normal_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        normals_attrib_location = shader->GetAttributeLocation(shader->normals_name);
        // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
        glEnableVertexAttribArray(normals_attrib_location);
        glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
    }
    
    if(shader->colors_name != ""){
        
        cout << "doing color" << endl;
        // bind buffer for color
        glBindBuffer(GL_ARRAY_BUFFER, buffers[color]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, color_coords.size()*sizeof(GLfloat), &color_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        colors_attrib_location = shader->GetAttributeLocation(shader->colors_name);
        // connect the color to the color attribute of the vertex shader
        glEnableVertexAttribArray(colors_attrib_location);
        glVertexAttribPointer(colors_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
    }
    
    
    if(shader->texture_coords_name != ""){
        
        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[texture]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, texture_coords.size()*sizeof(GLfloat), &texture_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        texture_coords_attrib_location = shader->GetAttributeLocation(shader->texture_coords_name);
        // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
        glEnableVertexAttribArray(texture_coords_attrib_location);
        glVertexAttribPointer(texture_coords_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    
    // unbind the VAO
    glBindVertexArray(0);
    
    cout << " done VBO and VAO" << endl;

}


void mesh::draw(){
    
    glBindVertexArray(vao[0]);
    
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4]);
    
    
    // draw strips
    for(int i=0;i<height-1;i++){
        glDrawElements(
                   GL_TRIANGLE_STRIP,      // mode
                   width*2,    // count
                   GL_UNSIGNED_INT,   // type
                   (void*) ((width*2*i)*sizeof(GLuint))          // element array buffer offset
                   );
    }
    
    /*
    // draw each strip
    // Draw the triangles !
    glDrawElements(
                   GL_TRIANGLE_STRIP,      // mode
                   indices.size(),    // count
                   GL_UNSIGNED_INT,   // type
                   (void*)0           // element array buffer offset
                   );
    */
}
