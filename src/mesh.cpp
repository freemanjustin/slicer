#include "mesh.h"


void mesh::init(ncio data, GLSLShader *shader){
    
    enum    {vertex, normal, color, texture, index};
    
    int i,j;
    float   r = 1.0f;
    float   lat, lon;
    
    // define the mesh here
    // including vertex position, texture coords, normals and/or colors
    
    // map the geolocated coords into cartesian coords.
    float min_value = *min_element(data.field.begin(), data.field.end());
    float max_value = *max_element(data.field.begin(), data.field.end());
    float range = max_value - min_value;
    
    width = data.nlon;
    height = data.nlat;
    
    for (j=height-1;j>=0;j--) {
        for (i=0;i<width;i++) { // longitude
    
            
            // normalized data field value
            float data_value = ( (data.field[ (width-i-1) + (width*j)]- min_value) / range);
            
            // set up vertex array
            // convert spherical coords to cartesian
            lat = data.lat[j] * M_PI/180.0f;
            lon = data.lon[i] * M_PI/180.0f;
            
            
            vertex_coords.push_back( (r+data_value*0.01) * -cos(lat) * sin(lon));   // x
            vertex_coords.push_back( (r+data_value*0.01) * sin(lat));    // y
            vertex_coords.push_back( (r+data_value*0.01) * -cos(lat) * cos(lon));             // z
            
            // set up texture coords
            texture_coords.push_back(1.0 - (float)(i)/(float)(width));
            texture_coords.push_back(1.0 - (float)j/(float)(height));
            
            
            // set vertex colors
            // this is a greyscale range
            
            //array[width * row + col]
            //x = data.field[ i + (width*j)] / range;
            //return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
            color_coords.push_back( ( (data.field[ i + (width*j)]- min_value) / range) );
            color_coords.push_back( ( (data.field[ i + (width*j)]- min_value) / range) );
            color_coords.push_back( ( (data.field[ i + (width*j)]- min_value) / range) );
            
            
        }
    }
    
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
    
}


void mesh::draw(){
    
    glBindVertexArray(vao[0]);
    
    // draw strips
    for(int i=0;i<height-1;i++){
        glDrawElements(GL_TRIANGLE_STRIP, width*2, GL_UNSIGNED_INT,
                       (void*) ((width*2*i)*sizeof(GLuint)));
    }
    
}
