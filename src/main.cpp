#define GLM_SWIZZLE

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#ifdef _OSX_
#include <GLUT/glut.h>
#endif

#include "camera.h"
#include "sphere.h"
#include "GLSLShader.h"

#include "SOIL.h"

using namespace std;

//Create the Camera
Camera camera;

Sphere ground;
Sphere sky;
Sphere texSphere;

// rotation variables
GLfloat XrotationAngle=0;
GLfloat YrotationAngle=0;
GLfloat ZrotationAngle=0;

// shader variables
GLSLShader groundFromSpace;
GLSLShader skyFromSpace;
GLSLShader texMap;

// texture map
GLuint  textures[2];


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


class Window {
public:
	Window() {
		this->interval = 1000 / 60;		//60 FPS
		this->window_handle = -1;
	}
	int window_handle, interval;
    glm::ivec2 size;
	float window_aspect;
} window;



void loadTextureMap(){
    
    textures[0] = SOIL_load_OGL_texture(
                                        "bluemarble/cont8.png",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y
                                        );
    
    if( textures[0] == 0 ){
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    textures[1] = SOIL_load_OGL_texture(
                                        "texture/test.jpg",
                                        SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y
                                        );
    
    if( textures[1] == 0 ){
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
}

//Invalidate the window handle when window is closed
void CloseFunc() {
	window.window_handle = -1;
}

//Resize the window and properly update the camera viewport
void ReshapeFunc(int w, int h) {
	if (h > 0) {
        window.size = glm::ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}
	camera.SetViewport(0, 0, window.size.x, window.size.y);
}


//Keyboard input for camera, also handles exit case
void KeyboardFunc(unsigned char c, int x, int y) {
    
    glm::vec3 lookAt;
    
	switch (c) {
        case 'j':
            lookAt = camera.camera_look_at;
            cout << "before: " << camera.camera_look_at.x << " " << camera.camera_look_at.y << " " << camera.camera_look_at.z << endl;
            camera.camera_position = glm::rotate(camera.camera_position, -0.1f, glm::vec3(0,1,0)); //rotating y axis
            cout << "after: " << camera.camera_look_at.x << " " << camera.camera_look_at.y << " " << camera.camera_look_at.z << endl;
            //camera.SetLookAt(glm::vec3(0, 0, 0));
            //camera.SetLookAt(lookAt);
            break;
        case 'l':
            camera.camera_position = glm::rotate(camera.camera_position, 0.1f, glm::vec3(0,1,0)); //rotating y axis
            camera.SetLookAt(glm::vec3(0, 0, 0));
            break;
        case 'w':
            camera.Move(FORWARD);
            break;
        case 'a':
            camera.Move(LEFT);
            //camera.SetLookAt(glm::vec3(0, 0, 0));
            break;
        case 's':
            camera.Move(BACK);
            break;
        case 'd':
            camera.Move(RIGHT);
            //camera.SetLookAt(glm::vec3(0, 0, 0));
            break;
        case 'q':
            camera.Move(DOWN);
            break;
        case 'e':
            camera.Move(UP);
            break;
        case 'x':
            XrotationAngle += 0.025;
            if(XrotationAngle > (360.0*M_PI/180.0) ) XrotationAngle -= (360.0*M_PI/180.0) ;
            break;
        case 'X':
            XrotationAngle -= 0.025;
            if(XrotationAngle < 0) XrotationAngle += 360.0*M_PI/180.0;
            break;
        case 'y':
            YrotationAngle += 0.025;
            if(YrotationAngle > (360.0*M_PI/180.0) ) YrotationAngle -= (360.0*M_PI/180.0) ;
            break;
        case 'Y':
            YrotationAngle -= 0.025;
            if(YrotationAngle < 0) YrotationAngle += 360.0*M_PI/180.0;
            break;
        case 'z':
            ZrotationAngle += 0.025;
            if(ZrotationAngle > (360.0*M_PI/180.0) ) ZrotationAngle -= (360.0*M_PI/180.0) ;
            break;
        case 'Z':
            ZrotationAngle -= 0.025;
            if(ZrotationAngle < 0) ZrotationAngle += 360.0*M_PI/180.0;
            break;
        case 'r':
            camera.SetPosition(glm::vec3(0, 0, 2));
            camera.SetLookAt(glm::vec3(0, 0, 0));
            XrotationAngle = YrotationAngle = ZrotationAngle = 0.0;
            break;
        case 27:
            exit(0);
            return;
        default:
            break;
	}
}

void SpecialFunc(int c, int x, int y) {}
void CallBackPassiveFunc(int x, int y) {}
//Used when person clicks mouse
void CallBackMouseFunc(int button, int state, int x, int y) {
	camera.SetPos(button, state, x, y);
}

//Used when person drags mouse around
void CallBackMotionFunc(int x, int y) {
	camera.Move2D(x, y);
}


void DisplayFunc() {
    
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	glm::mat4 model, view, projection;
	camera.Update();
    
    //camera.camera_position = glm::rotate(camera.camera_position, -0.1f, glm::vec3(0,1,0)); //rotating y axis
    
	camera.GetMatricies(projection, view, model);

    //model = glm::rotate(model, XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    //model = glm::rotate(model, YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    //model = glm::rotate(model, ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
	glm::mat4 mvp = projection * view * model;	//Compute the mvp matrix
	
    glLoadMatrixf(glm::value_ptr(mvp));
	
    // Enables Depth Testing
    glEnable(GL_DEPTH_TEST);
    
    // The Type Of Depth Testing To Do
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_MULTISAMPLE);
    
    float camera_magnitude = glm::length(camera.camera_position);
    float camera_magnitude_squared = pow(camera_magnitude ,2.0f);
    
    groundFromSpace.enable();
        groundFromSpace.SetUniform("model", model );
        groundFromSpace.SetUniform("view", view );
        groundFromSpace.SetUniform("projection", projection );
        groundFromSpace.SetUniform("v3CameraPos", camera.camera_position );
        groundFromSpace.SetUniform("v3LightPos", glm::normalize(camera.camera_position) ); //light_direction);
        groundFromSpace.SetUniform("v3InvWavelength", m_fWavelength4_inv );
        groundFromSpace.SetUniform("fCameraHeight", camera_magnitude);
        groundFromSpace.SetUniform("fCameraHeight2", camera_magnitude_squared);
        groundFromSpace.SetUniform("fInnerRadius", m_fInnerRadius);
        groundFromSpace.SetUniform("fInnerRadius2", m_fInnerRadius2);
        groundFromSpace.SetUniform("fOuterRadius", m_fOuterRadius);
        groundFromSpace.SetUniform("fOuterRadius2", m_fOuterRadius2);
        groundFromSpace.SetUniform("fKrESun", m_Kr*m_ESun);
        groundFromSpace.SetUniform("fKmESun", m_Km*m_ESun);
        groundFromSpace.SetUniform("fKr4PI", m_Kr4PI);
        groundFromSpace.SetUniform("fKm4PI", m_Km4PI);
        groundFromSpace.SetUniform("fScale", m_fScale);
        groundFromSpace.SetUniform("fScaleDepth", m_fRayleighScaleDepth);
        groundFromSpace.SetUniform("fScaleOverScaleDepth", m_fScaleOverScaleDepth);
        groundFromSpace.SetUniform("g", m_g);
        groundFromSpace.SetUniform("g2", m_g2);
        groundFromSpace.SetUniform("nSamples", m_nSamples);
        groundFromSpace.SetUniform("fSamples", (float)m_nSamples);
    
        ground.draw();
    
    groundFromSpace.disable();
    
    
    skyFromSpace.enable();
        skyFromSpace.SetUniform("model", model );
        skyFromSpace.SetUniform("view", view );
        skyFromSpace.SetUniform("projection", projection );
        skyFromSpace.SetUniform("v3CameraPos", camera.camera_position );
        skyFromSpace.SetUniform("v3LightPos", glm::normalize(camera.camera_position) );//light_direction );
        skyFromSpace.SetUniform("v3InvWavelength", m_fWavelength4_inv );
        skyFromSpace.SetUniform("fCameraHeight", camera_magnitude);
        skyFromSpace.SetUniform("fCameraHeight2", camera_magnitude_squared);
        skyFromSpace.SetUniform("fInnerRadius", m_fInnerRadius);
        skyFromSpace.SetUniform("fInnerRadius2", m_fInnerRadius2);
        skyFromSpace.SetUniform("fOuterRadius", m_fOuterRadius);
        skyFromSpace.SetUniform("fOuterRadius2", m_fOuterRadius2);
        skyFromSpace.SetUniform("fKrESun", m_Kr*m_ESun);
        skyFromSpace.SetUniform("fKmESun", m_Km*m_ESun);
        skyFromSpace.SetUniform("fKr4PI", m_Kr4PI);
        skyFromSpace.SetUniform("fKm4PI", m_Km4PI);
        skyFromSpace.SetUniform("fScale", m_fScale);
        skyFromSpace.SetUniform("fScaleDepth", m_fRayleighScaleDepth);
        skyFromSpace.SetUniform("fScaleOverScaleDepth", m_fScaleOverScaleDepth);
        skyFromSpace.SetUniform("g", m_g);
        skyFromSpace.SetUniform("g2", m_g2);
        skyFromSpace.SetUniform("nSamples", m_nSamples);
        skyFromSpace.SetUniform("fSamples", (float)m_nSamples);
    
        glFrontFace(GL_CW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
        sky.draw();
    
        glDisable(GL_BLEND);
        glFrontFace(GL_CCW);
    
    skyFromSpace.disable();
    
    
    
    /*
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Sphere::drawSphere(64, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    */
    
    // texture map sphere
    // bind the texture and set the "tex" uniform in the fragment shader
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    
    texMap.enable();
    
        texMap.SetUniform("Texture0", 0); //set to 0 because the texture is bound to GL_TEXTURE0
        texMap.SetUniform("Texture1", 1);
        texMap.SetUniform("model", model );
        texMap.SetUniform("view", view );
        texMap.SetUniform("projection", projection );
    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
        texSphere.draw();
    
        glDisable(GL_BLEND);
    texMap.disable();
    
    glDisable(GL_BLEND);
    texMap.disable();
    
    
    glutSwapBuffers();
}

//Redraw based on fps set for window
void TimerFunc(int value) {
	if (window.window_handle != -1) {
		glutTimerFunc(window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}




int main(int argc, char **argv) {
    //glut boilerplate
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    //Setup window and callbacks
    window.window_handle = glutCreateWindow("slicer");
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);
    glutMouseFunc(CallBackMouseFunc);
    glutMotionFunc(CallBackMotionFunc);
    glutPassiveMotionFunc(CallBackPassiveFunc);
    glutTimerFunc(window.interval, TimerFunc, 0);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "GLEW failed to initialize." << endl;
        exit(1);
    }
    if (!glewIsSupported("GL_VERSION_3_2")){
        cerr << "OpenGL 3.2 not supported." << endl;
        exit(1);
    }
    
    //Setup camera
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    camera.SetLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.SetClipping(.001f, 100.0f);
    camera.SetFOV(45.0f);
    
    
    // load the shaders
    groundFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromSpaceVert.glsl");
    groundFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromSpaceFrag.glsl");
    groundFromSpace.CreateAndLinkProgram();
    
    skyFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromSpaceVert.glsl");
    skyFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromSpaceFrag.glsl");
    skyFromSpace.CreateAndLinkProgram();

    
    
    // shader variables
    m_nSamples = 16;		// Number of sample rays to use in integral equation
    m_Kr = 0.0035f;		// Rayleigh scattering constant
    m_Kr4PI = m_Kr*4.0f*M_PI;
    m_Km = 0.0015f;		// Mie scattering constant
    m_Km4PI = m_Km*4.0f*M_PI;
    m_ESun = 6.0f;		// was 10 // Sun brightness constant
    m_g = -0.85f;		// The Mie phase asymmetry factor
    m_g2 = m_g*m_g;
    
    m_fInnerRadius = 1.0f;
    m_fInnerRadius2 = m_fInnerRadius*m_fInnerRadius;
    m_fOuterRadius = 1.025f;
    m_fOuterRadius2 = m_fOuterRadius*m_fOuterRadius;
    m_fScale = 1.0f / (m_fOuterRadius - m_fInnerRadius);
    
    
    m_fWavelength = glm::vec3(0.650f, 0.590f, 0.475f);
    // 650 nm for red
    // 570 nm for green
    // 475 nm for blue
    m_fWavelength4 = glm::pow(m_fWavelength, glm::vec3(4.0f,4.0f,4.0f));
    m_fWavelength4_inv = 1.0f/m_fWavelength4;
    
    m_fRayleighScaleDepth = 0.25f;
    m_fMieScaleDepth = 0.1f;
    
    m_fScaleOverScaleDepth = m_fScale / m_fRayleighScaleDepth;

    light_pos = glm::vec3(0.0, 0.0, -4.2);
    light_direction = glm::normalize(light_pos);
    
    // init sphere
    ground.vertex_position_attrib_location = groundFromSpace.GetAttributeLocation("v3Position");
    ground.texture_coords_attrib_location = groundFromSpace.GetAttributeLocation("v2TexCoords");
    ground.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    
    sky.vertex_position_attrib_location = skyFromSpace.GetAttributeLocation("v3Position");
    sky.texture_coords_attrib_location = skyFromSpace.GetAttributeLocation("v2TexCoords");
    sky.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fOuterRadius);
    
    
    /*
    // texture mapping
    texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/textureMap.vert");
    texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/textureMap.frag");
    texMap.CreateAndLinkProgram();
    
    texSphere.vertex_position_attrib_location = texMap.GetAttributeLocation("v3Position");
    texSphere.texture_coords_attrib_location = texMap.GetAttributeLocation("v2TexCoord");
    texSphere.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    */
    
    
    
    texMap.LoadFromFile(GL_VERTEX_SHADER,"shaders/texBlend_contrast.vert");
    texMap.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/texBlend_contrast.frag");
    texMap.CreateAndLinkProgram();
    
    texSphere.vertex_position_attrib_location = texMap.GetAttributeLocation("v3Position");
    texSphere.texture_coords_attrib_location = texMap.GetAttributeLocation("v2TexCoord");
    texSphere.init(200, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    
    loadTextureMap();
    
    
    //Start the glut loop!
    glutMainLoop();
    return 0;
}