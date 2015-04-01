#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include "camera.h"
#include "sphere.h"

using namespace std;

//Create the Camera
Camera camera;

// create a Sphere
Sphere sphere;

// rotation variables
GLfloat XrotationAngle=0;
GLfloat YrotationAngle=0;
GLfloat ZrotationAngle=0;

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
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);

	glm::mat4 model, view, projection;
	camera.Update();
	camera.GetMatricies(projection, view, model);

    model = glm::rotate(model, XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    model = glm::rotate(model, YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    model = glm::rotate(model, ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
	glm::mat4 mvp = projection* view * model;	//Compute the mvp matrix
	
    glLoadMatrixf(glm::value_ptr(mvp));
	
    // draw wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    sphere.draw();
    
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
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
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
		camera.SetPosition(glm::vec3(0, 0, -2.2));
		camera.SetLookAt(glm::vec3(0, 0, 0));
		camera.SetClipping(.1, 1000);
		camera.SetFOV(45);
    
        // init the sphere
        // needs to happen after we have an opengl context
        sphere.init(64, 0.0f, 0.0f, 0.0f, 1.0);
    
        //Start the glut loop!
		glutMainLoop();
        return 0;
}