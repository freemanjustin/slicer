#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSLShader.h"

#ifndef NETCDFIO_H
#include "netcdfIO.h"
#endif

class mesh{
    
    //int     resolution;
    //double  radius;
    int     width;
    int     height;
    
    std::vector<GLfloat> vertex_coords;
    std::vector<GLfloat> texture_coords;
    std::vector<GLfloat> normal_coords;
    std::vector<GLfloat> color_coords;
    std::vector<GLuint>  indices;
    
    GLuint  buffers[5];
    
    //GLSLShader  *shader;

    public:
        void init(ncio, GLSLShader*);
        void draw();
        
        GLint  vertex_position_attrib_location;
        GLint  texture_coords_attrib_location;
        GLint  normals_attrib_location;
        GLint  colors_attrib_location;
        
        GLuint  vao[1];
};