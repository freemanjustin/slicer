#include "lines.h"
#include <netcdf.h>

#define ERR(e) {cerr << "Error in netcdf load" << endl;}

void lines::init(GLSLShader *shader){
    
    enum    {vertex, normal, color, texture, index};
    
    float   r = 1.005f;
    float   lat, lon;
    glm::vec3   pos;
    glm::vec3   colors;
    
    
    
    ifstream    data_file;
    
    //int  idx = 0;
    //int  count = 0;
    //std::vector<int> startSegmentIndex;
    
    //data_file.open("continent/AustraliaContinentOutline.txt");
    data_file.open("continent/gshhg-bin-2.3.4/intermediate.txt");
    
    for(string line; getline(data_file, line); ){
        
        istringstream in(line);      //make a stream for the line itself
        
        float x, y, z;
        in >> x >> y >> z;       //now read the whitespace-separated floats
        
        //cout << x << " " << y << " " << " " <<z << endl;
        
        if (x == y == 999.0){
            pos.x = pos.y = pos.z = 0.0;
            //startSegmentIndex.push_back(idx);
            //idx++;
            //vertex_coords.push_back(pos);
        }
        else{
            // convert to opengl coords
            lat = y * M_PI/180.0f;
            lon = x * M_PI/180.0f;
            
            // map each point to a position in our scene
            pos.x = ( (r) * -cos(lat) * sin(lon));    // x
            pos.y = ( (r) * sin(lat));                // y
            pos.z = ( (r) * -cos(lat) * cos(lon));    // z
            //count++;
        }
        
        vertex_coords.push_back(pos);
        
        //cout << pos.x << " " << pos.y << " " << " " << pos.z << endl;
        
        // set the color to white
        colors.x = ( 1.0 );
        colors.y = ( 1.0 );
        colors.z = ( 1.0 );
        color_coords.push_back(colors);

    }
    
    // make the last point equal to the first point
    //vertex_coords.push_back(vertex_coords[0]);
    
    
    
    
    
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
        glVertexAttribPointer(vertex_position_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
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
}


void lines::draw(){
    
    
    GLenum error;
    
    glBindVertexArray(vao[0]);
    
    glDrawArrays(GL_LINE_STRIP, 0, vertex_coords.size());
    
    
    
}