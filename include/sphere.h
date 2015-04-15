#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <math.h>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSLShader.h"

class Sphere {
    
    int     resolution;
    double  radius;
    
    std::vector<GLfloat> vertex_coords;
    std::vector<GLfloat> texture_coords;
    std::vector<GLfloat> normal_coords;
    std::vector<GLfloat> color_coords;
    
    //GLSLShader  *shader;

public:
    Sphere();
    ~Sphere();
    void init(int, glm::vec3, double, GLSLShader*);
    void draw();

    GLint  vertex_position_attrib_location;
    GLint  texture_coords_attrib_location;
    GLint  normals_attrib_location;
    GLint  colors_attrib_location;
    
    GLuint  vao[1];
    
};


#endif