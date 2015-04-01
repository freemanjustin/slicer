#include "sphere.h"
using namespace std;

Sphere::Sphere(){
    num_vertices = 0;
    resolution = 0;
    radius = 0.0;
}

Sphere::~Sphere(){
}



// Create a sphere centered at c, with radius r, and precision n
// Modified from the Paul Bourke implementation.
// see:
// http://paulbourke.net/geometry/circlesphere/

void Sphere::init(int n, double xc, double yc, double zc, double r)
{
    int i,j;
    double t1,t2,t3;
    float e[3],p[3], c[3];
    
    vector<float> vertex_array;
    vector<float> normal_array;
    vector<float> color_array;
    
    for (j=0;j<n/2;j++) {
        t1 = (j*M_PI / (n/2)) - (M_PI/2);
        t2 = ((j+1)*M_PI / (n/2)) - (M_PI/2);
        
        for (i=0;i<=n;i++) {
            t3 = 2*i*M_PI / n - (M_PI/2);
            
            e[0] = cos(t1) * cos(t3);
            e[1] = sin(t1);
            e[2] = cos(t1) * sin(t3);
            p[0] = xc + r * e[0];
            p[1] = yc + r * e[1];
            p[2] = zc + r * e[2];
            
            c[0] = fabs(e[0]);
            c[1] = fabs(e[1]);
            c[2] = fabs(e[2]);
            
            
            normal_array.insert(normal_array.end(), e, e+3);
            color_array.insert(color_array.end(),c,c+3);
            
            // j - swapped the x texture coordinates so that the
            // texture maps show up properly
            // this commented line is the original texCoord
            //glTexCoord2f(i/(double)n,2*j/(double)n);
            //glTexCoord2f( (n-i)/(double)n, 2*j/(double)n);
            
            vertex_array.insert(vertex_array.end(), p, p+3);
            
            
            e[0] = cos(t2) * cos(t3);
            e[1] = sin(t2);
            e[2] = cos(t2) * sin(t3);
            p[0] = xc + r * e[0];
            p[1] = yc + r * e[1];
            p[2] = zc + r * e[2];
            normal_array.insert(normal_array.end(), e, e+3);
            
            color_array.insert(color_array.end(),c,c+3);
            // j - swapped the x texture coordinates so that the
            // texture maps show up properly
            // this commented line is the original texCoord
            //glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
            //glTexCoord2f( (n-i)/(double)n,2*(j+1)/(double)n);
            vertex_array.insert(vertex_array.end(), p, p+3);
        }
    }
    
    radius = r;
    resolution = n;
    num_vertices = vertex_array.size();
    
    
    unsigned int bufferIDs[3];
    glGenBuffers(3, bufferIDs);
    
    vertexVboId = bufferIDs[0];
    normalVboId = bufferIDs[1];
    colorVboId = bufferIDs[2];
    
    // vertex position
    glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);
    glBufferData(GL_ARRAY_BUFFER, vertex_array.size()*sizeof(float), &vertex_array[0], GL_STATIC_DRAW);
    
    // normals
    glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
    glBufferData(GL_ARRAY_BUFFER, normal_array.size()*sizeof(float), &normal_array[0], GL_STATIC_DRAW);
    
    // color
    glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glBufferData(GL_ARRAY_BUFFER, color_array.size()*sizeof(float), &color_array[0], GL_STATIC_DRAW);
    
}

void Sphere::draw() {
    
    if(radius == 0.0) return;
    
    int i;
    int slices = resolution;
    int stacks = slices/2;
    int vertices = (slices+1)*2;
    
    glEnableClientState(GL_VERTEX_ARRAY);   // enable vertex
    glEnableClientState(GL_NORMAL_ARRAY);   // enable normals
    glEnableClientState(GL_COLOR_ARRAY);    // enable color array
    
    // When using VBOs, the pointers refer to data in the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);
    glVertexPointer(3,GL_FLOAT,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalVboId);
    glNormalPointer(GL_FLOAT,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorVboId);
    glColorPointer(3,GL_FLOAT,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    for (i = 0; i < stacks; i++) {
        int pos = i*(slices+1)*2;
        glDrawArrays(GL_TRIANGLE_STRIP, pos, vertices);
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
}
