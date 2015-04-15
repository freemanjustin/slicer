#include "mesh.h"


void mesh::init(ncio field, GLSLShader *shader){
    
    enum    {vertex, normal, color, texture, index};
    
    int i,j;
    float   radius = 1.01f; // fix this
    float   lat, lon;
    
    // define the mesh here
    // including vertex position, texture coords, normals and/or colors
    //vertex_coords.resize(3*(int)field.nlat*(int)field.nlon);
    //texture_coords.resize(2*(int)field.nlat*(int)field.nlon);
    //normal_coords.resize(3*(int)field.nlat*(int)field.nlon);
    //color_coords.resize(3*(int)field.nlat*(int)field.nlon);
    //indices.resize((int)field.nlat*(int)field.nlon);
    
    // map the geolocated coords into cartesian coords.
    
    for (j=0;j<field.nlat;j++) { // latitude
        for (i=0;i<field.nlon;i++) { // longitude
    
            // set up vertex array
            // convert spherical coords to cartesian
            lat = field.lat[j] * M_PI/180.0f;
            lon = field.lon[i] * M_PI/180.0f;
            vertex_coords.push_back(radius * cos(lat) * cos(lon));   // x
            vertex_coords.push_back(radius * cos(lat) * sin(lon));   // y
            vertex_coords.push_back(radius * sin(lat));              // z
            
            // set up texture coords
            texture_coords.push_back(1.0 - (float)(i)/(float)(field.nlon));
            texture_coords.push_back(1.0 - (float)j/(float)(field.nlat));
            
        }
    }
    
    cout << "done vertex" << endl;
    cout << "done normals" << endl;
    
    // setup triangle face list - this is done per triangle
    // for each triangle we just store the index into the vertex array
            
    for (j=0;j<field.nlat-1;j++) {		// latitude
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
            
    cout << "done indices" << endl;
            
            
    
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
    GLuint  buffers[5];
    glGenBuffers(4, buffers);
    
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
        
        
        
        /*
        // now bind the index buffer
        // Index buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffers[indices]);
        
        // This only needs to be done once as the indices never change
        glBufferData(GL_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        */
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
        
        // bind buffer for color
        glBindBuffer(GL_ARRAY_BUFFER, buffers[color]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, color_coords.size()*sizeof(GLfloat), &color_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        colors_attrib_location = shader->GetAttributeLocation(shader->colors_name);
        // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
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
    
    // do we need the sizeof arg below??
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
    
}
