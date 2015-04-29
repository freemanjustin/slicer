// slicer
//
// freeman.justin@gmail.com

#ifndef SLICER_H
#define SLICER_H

#define GLM_SWIZZLE

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#elif defined _LINUX_
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include "camera.h"
#include "sphere.h"
#include "texture.h"
#include "GLSLShader.h"
#include "save_image.h"
#include "mesh.h"


#define NCIO

#ifdef NCIO
#include "netcdfIO.h"
#endif

#include "SOIL.h"


using namespace std;

// just putting these here for now....
// glsl atmo shader params
class atmospheric_scatter{
    public:
        int m_nSamples;
        float m_Kr, m_Kr4PI;
        float m_Km, m_Km4PI;
        float m_ESun;
        float m_g;
        float m_g2;

        float m_fInnerRadius;
        float m_fInnerRadius2;

        float m_fOuterRadius;
        float m_fOuterRadius2;

        float m_fScale;
        glm::vec3 m_fWavelength;
        glm::vec3 m_fWavelength4;
        glm::vec3 m_fWavelength4_inv;
        float m_fRayleighScaleDepth;
        float m_fMieScaleDepth;
        float m_fScaleOverScaleDepth;

        glm::vec3 light_pos;
        glm::vec3 light_direction;
};

class Window {
    public:
        Window() {
            this->interval = 1000 / 60;		//60 FPS
            this->window_handle = -1;
        }
        int window_handle, interval;
        glm::ivec2 size;
        float window_aspect;
};

class slicer{

    public:
    
        bool drawThis;
    
        // window
        Window  window;
    
        //Create the Camera
        Camera camera;
    
        // our spheres
        Sphere ground;
        Sphere sky;
        Sphere texSphere;
    
    
        Sphere test_sphere;
        Sphere test_sphere2;
    
        mesh   bathy_mesh;
        mesh    bathy_mesh_normals;
        // shader variables
        GLSLShader groundFromSpace;
        GLSLShader skyFromSpace;
    
        GLSLShader groundFromAtmosphere;
        GLSLShader skyFromAtmosphere;
    
        GLSLShader texMap;
    
        GLSLShader passThrough;
    
        GLSLShader renderNormals;
    
        atmospheric_scatter as;
    
        // rotation variables
        GLfloat XrotationAngle;
        GLfloat YrotationAngle;
        GLfloat ZrotationAngle;
    
        // texture map
        texture  continents;
        texture  field;
    
    #ifdef NCIO
        // netcdf tsting
        // this will be trashed
        ncio    bathy;
    #endif
    
        void    init();

};

#endif