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

#ifdef _OS_X_
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
        // window
        Window  window;
    
        //Create the Camera
        Camera camera;
    
        // our spheres
        Sphere ground;
        Sphere sky;
        Sphere texSphere;
    
        // shader variables
        GLSLShader groundFromSpace;
        GLSLShader skyFromSpace;
    
        GLSLShader groundFromAtmosphere;
        GLSLShader skyFromAtmosphere;
    
        GLSLShader texMap;
        
        atmospheric_scatter as;
    
        // rotation variables
        GLfloat XrotationAngle;
        GLfloat YrotationAngle;
        GLfloat ZrotationAngle;
    
        // texture map
        texture  continents;
        texture  field;
    
        void    init();

};

void slicer::init(){
    
    XrotationAngle = 0.0f;
    YrotationAngle = 0.0f;
    ZrotationAngle = 0.0f;
    
    //Setup camera
    //camera.SetMode(SPHERICAL);
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.SetClipping(.001f, 100.0f);
    camera.SetFOV(45.0f);
    
    
    // load the shaders
    groundFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromSpaceVert.glsl");
    groundFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromSpaceFrag.glsl");
    groundFromSpace.CreateAndLinkProgram();
    groundFromSpace.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    
    skyFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromSpaceVert.glsl");
    skyFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromSpaceFrag.glsl");
    skyFromSpace.CreateAndLinkProgram();
    skyFromSpace.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    
    groundFromAtmosphere.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromAtmosphereVert.glsl");
    groundFromAtmosphere.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromAtmosphereFrag.glsl");
    groundFromAtmosphere.CreateAndLinkProgram();
    groundFromAtmosphere.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    
    skyFromAtmosphere.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromAtmosphereVert.glsl");
    skyFromAtmosphere.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromAtmosphereFrag.glsl");
    skyFromAtmosphere.CreateAndLinkProgram();
    skyFromAtmosphere.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    
    
    // shader variables
    as.m_nSamples = 32;		// Number of sample rays to use in integral equation
    as.m_Kr = 0.0035f;		// Rayleigh scattering constant
    as.m_Kr4PI = as.m_Kr*4.0f*M_PI;
    as.m_Km = 0.0015f;		// Mie scattering constant
    as.m_Km4PI = as.m_Km*4.0f*M_PI;
    as.m_ESun = 6.0f;		// was 10 // Sun brightness constant
    as.m_g = -0.85f;		// The Mie phase asymmetry factor
    as.m_g2 = as.m_g*as.m_g;
    
    as.m_fInnerRadius = 1.0f;
    as.m_fInnerRadius2 = as.m_fInnerRadius*as.m_fInnerRadius;
    as.m_fOuterRadius = 1.025f;
    as.m_fOuterRadius2 = as.m_fOuterRadius*as.m_fOuterRadius;
    as.m_fScale = 1.0f / (as.m_fOuterRadius - as.m_fInnerRadius);
    
    
    as.m_fWavelength = glm::vec3(0.650f, 0.590f, 0.475f);
    // 650 nm for red
    // 570 nm for green
    // 475 nm for blue
    as.m_fWavelength4 = glm::pow(as.m_fWavelength, glm::vec3(4.0f,4.0f,4.0f));
    as.m_fWavelength4_inv = 1.0f/as.m_fWavelength4;
    
    as.m_fRayleighScaleDepth = 0.25f;
    as.m_fMieScaleDepth = 0.1f;
    
    as.m_fScaleOverScaleDepth = as.m_fScale / as.m_fRayleighScaleDepth;
    
    as.light_pos = glm::vec3(0.0, 0.0, -4.2);
    as.light_direction = glm::normalize(as.light_pos);
    
    // init sphere
    ground.init(200, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fInnerRadius, &groundFromSpace);
    sky.init(200, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fOuterRadius, &skyFromSpace);
    
    
    /*
     // texture mapping
     texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/textureMap.vert");
     texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/textureMap.frag");
     texMap.CreateAndLinkProgram();
     texMap.SetAttributeName(vertex_coords,"v3Position");
     texMap.SetAttributeName(texture_coords,"v2TexCoord");
     
     texSphere.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
     */
    
    texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/texBlend_contrast.vert");
    texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/texBlend_contrast.frag");
    texMap.CreateAndLinkProgram();
    texMap.SetAttributeName(GLSLShader::vertex_coords,"v3Position");
    texMap.SetAttributeName(GLSLShader::texture_coords,"v2TexCoord");
    
    
    texSphere.init(300, glm::vec3(0.0f, 0.0f, 0.0f), as.m_fInnerRadius+0.005, &texMap);
    
    //loadTextureMap(E);
    // load two sample texture maps
    continents.load_texture("bluemarble/cont8.png");
    field.load_texture("texture/test.jpg");

}

#endif