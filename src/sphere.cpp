#include "sphere.h"
#include "GLSLShader.h"

using namespace std;

Sphere::Sphere(){
    resolution = 0;
    radius = 0.0;
    vertex_position_attrib_location = -1;
    texture_coords_attrib_location = -1;
    shader = NULL;
}

Sphere::~Sphere(){
}



// Create a sphere centered at c, with radius r, and precision n
// Modified from the Paul Bourke implementation.
// see:
// http://paulbourke.net/geometry/circlesphere/

void Sphere::init(int n, glm::vec3 centre, double r, GLSLShader *shader)
{
    enum    {vertex, normal, color, texture};
    
    int i,j;
    float t1,t2,t3;
    float e[3],p[3], c[3];
    
    for (j=0;j<n/2;j++) {
        t1 = (j*M_PI / (n/2)) - (M_PI/2);
        t2 = ((j+1)*M_PI / (n/2)) - (M_PI/2);
        
        for (i=0;i<=n;i++) {
            t3 = 2*i*M_PI / n - (M_PI/2);
            
            e[0] = cos(t1) * cos(t3);
            e[1] = sin(t1);
            e[2] = cos(t1) * sin(t3);
            p[0] = centre.x + r * e[0];
            p[1] = centre.y + r * e[1];
            p[2] = centre.z + r * e[2];
            
            c[0] = fabs(e[0]);
            c[1] = fabs(e[1]);
            c[2] = fabs(e[2]);
            
            
            normal_coords.insert(normal_coords.end(), e, e+3);
            color_coords.insert(color_coords.end(),c,c+3);
            // vertex data
            vertex_coords.insert(vertex_coords.end(), p, p+3);
            // texture coordinate data
            texture_coords.push_back((float)(n-i)/(float)n);
            texture_coords.push_back((float)2.0f*j/(float)n);
            
            e[0] = cos(t2) * cos(t3);
            e[1] = sin(t2);
            e[2] = cos(t2) * sin(t3);
            p[0] = centre.x + r * e[0];
            p[1] = centre.y + r * e[1];
            p[2] = centre.z + r * e[2];
            
            
            normal_coords.insert(normal_coords.end(), e, e+3);
            color_coords.insert(color_coords.end(),c,c+3);
            // vertex data
            vertex_coords.insert(vertex_coords.end(), p, p+3);
            // texture coordinate data
            texture_coords.push_back((float)(n-i)/(float)n);
            texture_coords.push_back(2.0*(j+1)/(float)n);
        }
    }
    
    radius = r;
    resolution = n;
    
    // create VBO's
    GLuint  buffers[4];
    glGenBuffers(4, buffers);
    
    // create VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    
    // create VBO
    //
    // each buffer corresponds to:
    //
    //      buffer[0] = vertices
    //      buffer[1] = normals
    //      buffer[2] = colors
    //      buffer[3] = texture_coords
    
    
    
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
    
}

void Sphere::draw() {
    
    if(radius == 0.0) return;
    
    int i;
    int slices = resolution;
    int stacks = slices/2;
    int vertices = (slices+1)*2;
    
    glBindVertexArray(vao[0]);
    for (i = 0; i < stacks; i++) {
        int pos = i*(slices+1)*2;
        glDrawArrays(GL_TRIANGLE_STRIP, pos, vertices);
    }
}

