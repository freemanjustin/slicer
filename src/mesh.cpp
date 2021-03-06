#include "mesh.h"

void mesh::init(ncio data, GLSLShader *shader){
    
    enum    {vertex, normal, color, texture, index};
    
    int i,j;
    float   r = 1.0f;
    float   lat, lon;
    
    //GLint   max;
    //glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max);
    //cout << "max vertices = " << max << endl;
    
    // define the mesh
    // including vertex position, texture coords, normals and/or colors
    
    width = data.nlon;
    height = data.nlat;
    
    /*
    // map the bathymetry to only values greater than -5.0
    // will give a flat ocean...
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude
            if (data.field[ i + (width*j)] <= 0.0){
                data.field[ i + (width*j)] = 0.0;
            }
            else
                data.field[ i + (width*j)] = log(data.field[ i + (width*j)]);
        }
    }
    */
    
    
    // map the geolocated coords into cartesian coords.
    float min_value = *min_element(data.field.begin(), data.field.end());
    float max_value = *max_element(data.field.begin(), data.field.end());
    float range = max_value - min_value;
    
    
    //cout << "width = " << width << " height = " << height << endl;
    
    glm::vec4 vert;
    glm::vec2 text_coords;
    glm::vec3 colors;
    
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude
    
            // normalized data field value
            float data_value =( (data.field[ i + (width*j)]- min_value) / range);
            
            // set up vertex array
            // convert spherical coords to cartesian
            lat = data.lat[j] * M_PI/180.0f;
            lon = data.lon[i] * M_PI/180.0f;
            
            // map each point to a position in our scene
            vert.x = ( (r+data_value*0.005f) * -cos(lat) * sin(lon));    // x
            vert.y = ( (r+data_value*0.005f) * sin(lat));                // y
            vert.z = ( (r+data_value*0.005f) * -cos(lat) * cos(lon));    // z
            vert.w = 1.0f;
            vertex_coords.push_back(vert);
            
            // set up texture coords
            text_coords.x = (1.0f - (float)(i)/(float)(width));
            text_coords.y = (1.0f - (float)j/(float)(height));
            texture_coords.push_back(text_coords);
            
            // set vertex colors
            // this is a greyscale range
            
            colors.x = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            colors.y = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            colors.z = ( ( (data.field[ i + (width*j)]- min_value) / range) );
            color_coords.push_back(colors);
        }
    }
    
    //cout << "we have " << vertex_coords.size()/3 << " vertex_coords" << endl;
    // setup triangle face list - this is done per triangle
    // for each triangle we just store the index into the vertex array
    for (j=1;j<height;j++) { // latitude
        // first triangle for this strip
        
        indices.push_back(j*width - width);
        indices.push_back(j*width);
        indices.push_back( (j*width - width) +1);
        
        for (i=1;i<width;i++) { // longitude
            indices.push_back(i+(j*width));
            indices.push_back(i+(j*width - width)+1);
        }
        indices.pop_back();
    }
    
    // triangle list - trianles[i] = index coords for this triangle
    std::vector<glm::vec3> triangles;
    int row=1;
    for(i=0;i<indices.size();i++){
        if(i==row*(width*2)-2){
            i+=2;
            row++;
        }
        triangles.push_back(glm::vec3(indices[i],indices[i+1],indices[i+2]));
    }
    triangles.pop_back(); // trash the last entry
    
    //cout << "nVertex = " << vertex_coords.size() << endl;
    //cout << "nIndices = " << indices.size() << endl;
    //cout << "ntriangles = " << triangles.size() << endl;
    
    // calculate vertex normals
    normal_coords.resize(vertex_coords.size(), glm::vec3(0.0f, 0.0f, 0.0f)); // initialize the normals to the zero vector
    std::vector<GLint>  seen;   // count of how many triangles this vertex is a member of
    seen.resize(vertex_coords.size(), 0);
    
    for (i = 0; i < triangles.size(); i+=2) {
        // down pointing triangle
        // the vertices of this triangle
        GLint ia = triangles[i].x;
        GLint ib = triangles[i].y;
        GLint ic = triangles[i].z;
        
        glm::vec3 norm = glm::cross(glm::vec3(vertex_coords[ib]) - glm::vec3(vertex_coords[ia]),
                                    glm::vec3(vertex_coords[ic]) - glm::vec3(vertex_coords[ia]));
        
        //cout<<"norm is "<<glm::to_string(norm)<<endl;
        
        int v[3];
        v[0] = ia;
        v[1] = ib;
        v[2] = ic;
        for (int j = 0; j < 3; j++) {
            GLint cur_v = v[j];
            normal_coords[cur_v] += norm;
            seen[cur_v]++;
        }
        
        // up pointing triangle
        // the vertices of this triangle:
        ia = triangles[i+1].x;
        ib = triangles[i+1].y;
        ic = triangles[i+1].z;
        
        norm = glm::cross(glm::vec3(vertex_coords[ic]) - glm::vec3(vertex_coords[ia]),
                                    glm::vec3(vertex_coords[ib]) - glm::vec3(vertex_coords[ia]));
        
        //cout<<"norm is "<<glm::to_string(norm)<<endl;
        
        v[0] = ia;
        v[1] = ib;
        v[2] = ic;
        for (int j = 0; j < 3; j++) {
            GLint cur_v = v[j];
            normal_coords[cur_v] += norm;
            seen[cur_v]++;
        }
    }
    
    // average the vertex normals
    for (i = 0; i < normal_coords.size(); i++) {
        if (seen[i] > 1) {
            // average
            normal_coords[i].x /= seen[i];
            normal_coords[i].y /= seen[i];
            normal_coords[i].z /= seen[i];
        }
        normal_coords[i] = glm::normalize(normal_coords[i]);
    }

    
    // color map testing
    
    /*
    struct RGB colorConverter(int hexValue)
    {
        struct RGB rgbColor;
        rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
        rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
        rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
        
        return rgbColor; 
    }
    */
    
    /*
    // color mapping
    
    int hexColor[9];
    float   xCoord[9];
    
    hexColor[0] =  0x352a87;
    hexColor[1] =  0x0363e1;
    hexColor[2] =  0x1485d4;
    hexColor[3] =  0x06a7c6;
    hexColor[4] =  0x38b99e;
    hexColor[5] =  0x92bf73;
    hexColor[6] =  0xd9ba56;
    hexColor[7] =  0xfcce2e;
    hexColor[8] =  0xf9fb0e;
    
    float red, green, blue;
    
    for(i=0;i<9;i++){
        red = ((hexColor[i] >> 16) & 0xFF) / 255.0f;
        green = ((hexColor[i] >> 8) & 0xFF) / 255.0f;
        blue = ((hexColor[i]) & 0xFF) / 255.0f;
    
        xCoord[i] = (float)i/8.0f;
        cout << xCoord[i] << "," << red << "," << green << "," << blue << ","<<endl;
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
    
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glGenBuffers failed: error " << error << std::endl;
        exit( 1);
    }

    
    // create VAO
    glGenVertexArrays(1, vao);
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glGenVertexArrays failed: error " << error << std::endl;
        exit( 1);
    }
    glBindVertexArray(vao[0]);
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "glBindVertexArray failed: error " << error << std::endl;
        exit( 1);
    }
    
    // set the VAO pointers to the VBO
    if(shader->vertex_coords_name != ""){
        
        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[vertex]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, vertex_coords.size()*sizeof(vertex_coords[0]), &vertex_coords[0], GL_STATIC_DRAW);
        // get the attribute location
        vertex_position_attrib_location = shader->GetAttributeLocation(shader->vertex_coords_name);
        // connect the vertex.xyz data to the "v3Position" attribute of the vertex shader
        glEnableVertexAttribArray(vertex_position_attrib_location);
        glVertexAttribPointer(vertex_position_attrib_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        
        // now bind the index buffer
        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
    }
    
    
    if(shader->normals_name != ""){
        
        // bind buffer for vertices
        glBindBuffer(GL_ARRAY_BUFFER, buffers[normal]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, normal_coords.size()*sizeof(normal_coords[0]), &normal_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        normals_attrib_location = shader->GetAttributeLocation(shader->normals_name);
        // connect the uv texture coords to the normal attribute of the vertex shader
        glEnableVertexAttribArray(normals_attrib_location);
        glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        //cout << "done normals" << " normals name is " << shader->normals_name << endl;
        
    }
    
    if(shader->colors_name != ""){
        
        // bind buffer for color
        glBindBuffer(GL_ARRAY_BUFFER, buffers[color]);
        // and copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, color_coords.size()*sizeof(color_coords[0]), &color_coords[0], GL_STATIC_DRAW);
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
        glBufferData(GL_ARRAY_BUFFER, texture_coords.size()*sizeof(texture_coords[0]), &texture_coords[0], GL_STATIC_DRAW);
        // get attribute location in shader
        texture_coords_attrib_location = shader->GetAttributeLocation(shader->texture_coords_name);
        // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
        glEnableVertexAttribArray(texture_coords_attrib_location);
        glVertexAttribPointer(texture_coords_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }
    
    // unbind the VAO
    glBindVertexArray(0);
    
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "?? failed: error " << error << std::endl;
        exit( 1);
    }
    
}


void mesh::draw(){
    
    
    
    GLenum error;
    /*
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << " pre bindvertexarray failed error = " << error << std::endl;
        //exit(1);
    }
    */
    
    glBindVertexArray(vao[0]);
    
    /*
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << " ### bindvertexarray failed error = " << error << std::endl;
        //exit(1);
    }
     */
    
    
        // draw strips
    for(int i=0;i<height-1;i++){
    //for(int i=0;i<(height/2)-1;i++){
        glDrawElements(GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT, (void*) ((width*2*i)*sizeof(GLuint)));
        //glDrawElements(GL_LINE_STRIP, width*2, GL_UNSIGNED_INT, (void*) ((width*2*i)*sizeof(GLuint)));
    }
    /*
    for(int i=(height/2)-1;i<(height)-1;i++){
        glDrawElements(GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT,
                       (void*) ((width*2*i)*sizeof(GLuint)));
    }
    */
    /*
    // check for errors
    error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cerr << "XX error " << error << std::endl;
        exit( 1);
    }
     */
    
}
