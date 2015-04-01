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

class Sphere {
    
    int num_vertices;
    int resolution;
    //double xc;
    //double yc;
    //double zc;
    double radius;
    int vertexVboId;   // identifier for the Vertex Buffer Object to hold the vertex coords
    int normalVboId;   // identifier for the Vertex Buffer Object to hold the normla vectors
    int colorVboId;   // identifier for the Vertex Buffer Object to hold the normla vectors

public:
    Sphere();
    ~Sphere();
    void init(int, double, double, double, double);
    void draw();
    
};


#endif