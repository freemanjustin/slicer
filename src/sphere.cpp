#include "sphere.h"
using namespace std;

Sphere::Sphere(){
    resolution = 0;
    radius = 0.0;
    vertex_position_attrib_location = -1;
    texture_coords_attrib_location = -1;
}

Sphere::~Sphere(){
}



// Create a sphere centered at c, with radius r, and precision n
// Modified from the Paul Bourke implementation.
// see:
// http://paulbourke.net/geometry/circlesphere/

void Sphere::init(int n, glm::vec3 centre, double r)
{
    int i,j;
    float t1,t2,t3;
    float e[3],p[3], c[3];
    
    //vector<GLfloat> buffer_data;
    
    
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
            
            
            //normal_array.insert(normal_array.end(), e, e+3);
            //color_array.insert(color_array.end(),c,c+3);
            
            // j - swapped the x texture coordinates so that the
            // texture maps show up properly
            // this commented line is the original texCoord
            //glTexCoord2f(i/(double)n,2*j/(double)n);
            //glTexCoord2f( (n-i)/(double)n, 2*j/(double)n);
            
            //vertex_array.insert(vertex_array.end(), p, p+3);
            
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
            //normal_array.insert(normal_array.end(), e, e+3);
            
            //color_array.insert(color_array.end(),c,c+3);
            // j - swapped the x texture coordinates so that the
            // texture maps show up properly
            // this commented line is the original texCoord
            //glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
            //glTexCoord2f( (n-i)/(double)n,2*(j+1)/(double)n);
            //vertex_array.insert(vertex_array.end(), p, p+3);
            
            // vertex data
            vertex_coords.insert(vertex_coords.end(), p, p+3);
            // texture coordinate data
            texture_coords.push_back((float)(n-i)/(float)n);
            texture_coords.push_back(2.0*(j+1)/(float)n);
        }
    }
    
    radius = r;
    resolution = n;
    
    
    GLuint  buffers[2];
    
    // create VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    
    // crate VBO
    glGenBuffers(2, buffers);
    // bind buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    // and copy data into buffer
    glBufferData(GL_ARRAY_BUFFER, vertex_coords.size()*sizeof(GLfloat), &vertex_coords[0], GL_STATIC_DRAW);
    
    // connect the vertex.xyz data to the "v3Position" attribute of the vertex shader
    glEnableVertexAttribArray(vertex_position_attrib_location);
    glVertexAttribPointer(vertex_position_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // bind buffer for vertices
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    // and copy data into buffer
    glBufferData(GL_ARRAY_BUFFER, texture_coords.size()*sizeof(GLfloat), &texture_coords[0], GL_STATIC_DRAW);
    
    // connect the uv texture coords to the "v2TextCoord" attribute of the vertex shader
    glEnableVertexAttribArray(texture_coords_attrib_location);
    glVertexAttribPointer(texture_coords_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
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

void Sphere::drawSphere(int n, glm::vec3 centre, double r){
    
    Sphere S;
    S.init(n,centre,r);
    S.draw();
    
}

/*
GLint Sphere::getVertexVBOid(){
    
    return vertexVboId;
}
 */
