#define GLM_SWIZZLE

#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include "camera.h"
#include "sphere.h"
#include "GLSLShader.h"

using namespace std;

//Create the Camera
Camera camera;

// rotation variables
GLfloat XrotationAngle=0;
GLfloat YrotationAngle=0;
GLfloat ZrotationAngle=0;

// shader variables
GLSLShader groundFromSpace;
GLSLShader skyFromSpace;

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
float m_fWavelength[3];
float m_fWavelength4[3];
float m_fWavelength4_inv[3];
float m_fRayleighScaleDepth;
float m_fMieScaleDepth;
float m_fScaleOverScaleDepth;


float light_pos[4];
float light_magnitude;
float light_direction[3];




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
	switch (c) {
        case 'w':
            camera.Move(FORWARD);
            break;
        case 'a':
            camera.Move(LEFT);
            break;
        case 's':
            camera.Move(BACK);
            break;
        case 'd':
            camera.Move(RIGHT);
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
            camera.SetPosition(glm::vec3(0, 0, -4));
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
//Draw a wire cube! (nothing fancy here)
void DisplayFunc() {
    
    float	camera_magnitude;
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	glm::mat4 model, view, projection;
	camera.Update();
	camera.GetMatricies(projection, view, model);

    //model = glm::rotate(model, XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    //model = glm::rotate(model, YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    //model = glm::rotate(model, ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
	glm::mat4 mvp = projection* view * model;	//Compute the mvp matrix
	
    glLoadMatrixf(glm::value_ptr(mvp));
	
    // Enables Depth Testing
    glEnable(GL_DEPTH_TEST);
    
    // The Type Of Depth Testing To Do
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_MULTISAMPLE);
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    camera_magnitude = sqrt(camera.camera_position.x*camera.camera_position.x + camera.camera_position.y*camera.camera_position.y + camera.camera_position.z*camera.camera_position.z);
    
    //cout << camera.camera_position.x << " " << camera.camera_position.y<< " " << camera.camera_position.z << endl;
    
    groundFromSpace.enable();
        glUniform3f(groundFromSpace("v3CameraPos"), camera.camera_position.x, camera.camera_position.y, camera.camera_position.z);
        glUniform3f(groundFromSpace("v3LightPos"), light_direction[0], light_direction[1], light_direction[2]);   // fix this one
        glUniform3f(groundFromSpace("v3InvWavelength"), m_fWavelength4_inv[0], m_fWavelength4_inv[1], m_fWavelength4_inv[2]);
        glUniform3f(groundFromSpace("v3InvWavelength"), m_fWavelength4_inv[0], m_fWavelength4_inv[1], m_fWavelength4_inv[2]);
        glUniform1f(groundFromSpace("fCameraHeight"), camera_magnitude);
        glUniform1f(groundFromSpace("fCameraHeight2"), camera_magnitude*camera_magnitude);
        glUniform1f(groundFromSpace("fInnerRadius"), m_fInnerRadius);
        glUniform1f(groundFromSpace("fInnerRadius2"), m_fInnerRadius2);
        glUniform1f(groundFromSpace("fOuterRadius"), m_fOuterRadius);
        glUniform1f(groundFromSpace("fOuterRadius2"), m_fOuterRadius2);
        glUniform1f(groundFromSpace("fKrESun"), m_Kr*m_ESun);
        glUniform1f(groundFromSpace("fKmESun"), m_Km*m_ESun);
        glUniform1f(groundFromSpace("fKr4PI"), m_Kr4PI);
        glUniform1f(groundFromSpace("fKm4PI"), m_Km4PI);
        glUniform1f(groundFromSpace("fScale"), m_fScale);
        glUniform1f(groundFromSpace("fScaleDepth"), m_fRayleighScaleDepth);
        glUniform1f(groundFromSpace("fScaleOverScaleDepth"), m_fScaleOverScaleDepth);
        glUniform1f(groundFromSpace("g"), m_g);
        glUniform1f(groundFromSpace("g2"), m_g2);
        glUniform1i(groundFromSpace("nSamples"), m_nSamples);
        glUniform1f(groundFromSpace("fSamples"), m_nSamples);
    
        Sphere::drawSphere(128, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    groundFromSpace.disable();
    
    
    skyFromSpace.enable();
        glUniform3f(skyFromSpace("v3CameraPos"), camera.camera_position.x, camera.camera_position.y, camera.camera_position.z);
        glUniform3f(skyFromSpace("v3LightPos"), light_direction[0], light_direction[1], light_direction[2]);   // fix this one
        glUniform3f(skyFromSpace("v3InvWavelength"), m_fWavelength4_inv[0], m_fWavelength4_inv[1], m_fWavelength4_inv[2]);
        glUniform3f(skyFromSpace("v3InvWavelength"), m_fWavelength4_inv[0], m_fWavelength4_inv[1], m_fWavelength4_inv[2]);
        glUniform1f(skyFromSpace("fCameraHeight"), camera_magnitude);
        glUniform1f(skyFromSpace("fCameraHeight2"), camera_magnitude*camera_magnitude);
        glUniform1f(skyFromSpace("fInnerRadius"), m_fInnerRadius);
        glUniform1f(skyFromSpace("fInnerRadius2"), m_fInnerRadius2);
        glUniform1f(skyFromSpace("fOuterRadius"), m_fOuterRadius);
        glUniform1f(skyFromSpace("fOuterRadius2"), m_fOuterRadius2);
        glUniform1f(skyFromSpace("fKrESun"), m_Kr*m_ESun);
        glUniform1f(skyFromSpace("fKmESun"), m_Km*m_ESun);
        glUniform1f(skyFromSpace("fKr4PI"), m_Kr4PI);
        glUniform1f(skyFromSpace("fKm4PI"), m_Km4PI);
        glUniform1f(skyFromSpace("fScale"), m_fScale);
        glUniform1f(skyFromSpace("fScaleDepth"), m_fRayleighScaleDepth);
        glUniform1f(skyFromSpace("fScaleOverScaleDepth"), m_fScaleOverScaleDepth);
        glUniform1f(skyFromSpace("g"), m_g);
        glUniform1f(skyFromSpace("g2"), m_g2);
        glUniform1i(skyFromSpace("nSamples"), m_nSamples);
        glUniform1f(skyFromSpace("fSamples"), m_nSamples);
    
        glFrontFace(GL_CW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        Sphere::drawSphere(128, glm::vec3(0.0f, 0.0f, 0.0f), m_fOuterRadius);
        glDisable(GL_BLEND);
        glFrontFace(GL_CCW);
    skyFromSpace.disable();
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Sphere::drawSphere(128, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
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
    //Setup camera
    camera.SetMode(FREE);
    camera.SetPosition(glm::vec3(0, 0, -4.2));
    camera.SetLookAt(glm::vec3(0, 0, 0));
    camera.SetClipping(.1, 1000);
    camera.SetFOV(45);
    
    // load the shaders
    groundFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/GroundFromSpaceVert.glsl");
    groundFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/GroundFromSpaceFrag.glsl");
    groundFromSpace.CreateAndLinkProgram();
    // set uniforms for this shader
    groundFromSpace.AddUniform("v3CameraPos");
    groundFromSpace.AddUniform("v3LightPos");
    groundFromSpace.AddUniform("v3InvWavelength");
    groundFromSpace.AddUniform("fCameraHeight");
    groundFromSpace.AddUniform("fCameraHeight2");
    groundFromSpace.AddUniform("fInnerRadius");
    groundFromSpace.AddUniform("fInnerRadius2");
    groundFromSpace.AddUniform("fOuterRadius");
    groundFromSpace.AddUniform("fOuterRadius2");
    groundFromSpace.AddUniform("fKrESun");
    groundFromSpace.AddUniform("fKmESun");
    groundFromSpace.AddUniform("fKr4PI");
    groundFromSpace.AddUniform("fKm4PI");
    groundFromSpace.AddUniform("fScale");
    groundFromSpace.AddUniform("fScaleDepth");
    groundFromSpace.AddUniform("fScaleOverScaleDepth");
    groundFromSpace.AddUniform("g");
    groundFromSpace.AddUniform("g2");
    groundFromSpace.AddUniform("nSamples");
    groundFromSpace.AddUniform("fSamples");
    
    
    
    skyFromSpace.LoadFromFile(GL_VERTEX_SHADER,"shaders/SkyFromSpaceVert.glsl");
    skyFromSpace.LoadFromFile(GL_FRAGMENT_SHADER,"shaders/SkyFromSpaceFrag.glsl");
    skyFromSpace.CreateAndLinkProgram();
    // set uniforms for this shader
    skyFromSpace.AddUniform("v3CameraPos");
    skyFromSpace.AddUniform("v3LightPos");
    skyFromSpace.AddUniform("v3InvWavelength");
    skyFromSpace.AddUniform("fCameraHeight");
    skyFromSpace.AddUniform("fCameraHeight2");
    skyFromSpace.AddUniform("fInnerRadius");
    skyFromSpace.AddUniform("fInnerRadius2");
    skyFromSpace.AddUniform("fOuterRadius");
    skyFromSpace.AddUniform("fOuterRadius2");
    skyFromSpace.AddUniform("fKrESun");
    skyFromSpace.AddUniform("fKmESun");
    skyFromSpace.AddUniform("fKr4PI");
    skyFromSpace.AddUniform("fKm4PI");
    skyFromSpace.AddUniform("fScale");
    skyFromSpace.AddUniform("fScaleDepth");
    skyFromSpace.AddUniform("fScaleOverScaleDepth");
    skyFromSpace.AddUniform("g");
    skyFromSpace.AddUniform("g2");
    skyFromSpace.AddUniform("nSamples");
    skyFromSpace.AddUniform("fSamples");

    
    // shader constants
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
    
    
    m_fWavelength[0] = 0.650f;		// 650 nm for red
    m_fWavelength[1] = 0.590f;		// 570 nm for green
    m_fWavelength[2] = 0.475f;		// 475 nm for blue
    
    
    /*
     // ...white atmosphere...
     m_fWavelength[0] = 0.620f;		// 650 nm for red
     m_fWavelength[1] = 0.620f;		// 570 nm for green
     m_fWavelength[2] = 0.620f;		// 475 nm for blue
     */
    
    m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
    m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
    m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);
    
    m_fWavelength4_inv[0] = 1.0/m_fWavelength4[0];
    m_fWavelength4_inv[1] = 1.0/m_fWavelength4[1];
    m_fWavelength4_inv[2] = 1.0/m_fWavelength4[2];
    
    m_fRayleighScaleDepth = 0.25f;
    m_fMieScaleDepth = 0.1f;
    
    m_fScaleOverScaleDepth = m_fScale / m_fRayleighScaleDepth;

    
    light_pos[0] = 0.0;
    light_pos[1] = 0.0;
    light_pos[2] = -4.2;
    
    
    light_magnitude = sqrt(light_pos[0]*light_pos[0] +
                           light_pos[1]*light_pos[1] +
                           light_pos[2]*light_pos[2]);
    
    light_direction[0] = light_pos[0]/light_magnitude;
    light_direction[1] = light_pos[1]/light_magnitude;
    light_direction[2] = light_pos[2]/light_magnitude;
    
    //Start the glut loop!
    glutMainLoop();
    return 0;
}