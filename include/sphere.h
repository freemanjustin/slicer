#ifndef SPHERE_H
#define SPHERE_H

#ifdef _OSX_
    #include <GLUT/glut.h>
#else
    #include <GL/glew.h>
#endif

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


class Sphere {
    
    int     num_vertices;
    int     resolution;
    double  radius;
    
    int     vertexVboId;   // identifier for the Vertex Buffer Object to hold the vertex coords
    int     normalVboId;   // identifier for the Vertex Buffer Object to hold the normla vectors
    int     colorVboId;   // identifier for the Vertex Buffer Object to hold the normla vectors
    int     textureVboId;

public:
    Sphere();
    ~Sphere();
    void init(int, glm::vec3, double);
    void draw();
    static void drawSphere(int n, glm::vec3 centre, double r);
    
};


#endif