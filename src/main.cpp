#include "slicer.h"

slicer *E;

//Invalidate the window handle when window is closed
void CloseFunc() {
	E->window.window_handle = -1;
}

//Resize the window and properly update the camera viewport
void ReshapeFunc(int w, int h) {
	if (h > 0) {
        E->window.size = glm::ivec2(w, h);
		E->window.window_aspect = float(w) / float(h);
	}
	E->camera.SetViewport(0, 0, E->window.size.x, E->window.size.y);
}


//Keyboard input for camera, also handles exit case
void KeyboardFunc(unsigned char c, int x, int y) {
    
	switch (c) {
        case 'v':
            WindowDump_PNG();
            break;
        case 'j':
            E->camera.camera_position = glm::rotate(E->camera.camera_position, 0.1f, glm::vec3(0.0f,1.0f,0.0f)); //rotating y axis
            E->camera.camera_look_at = glm::rotate(E->camera.camera_look_at, 0.1f, glm::vec3(0.0f,1.0f,0.0f)); //rotating y axis
            //cout << E->camera.camera_position.x << endl;
            //cout << E->camera.camera_position.y<< endl;
            //cout << E->camera.camera_position.z<< endl;
            break;
        case 'l':
            E->camera.camera_position = glm::rotate(E->camera.camera_position, -0.1f, glm::vec3(0.0f,1.0f,0.0f)); //rotating y axis
            E->camera.camera_look_at = glm::rotate(E->camera.camera_look_at, -0.1f, glm::vec3(0.0f,1.0f,0.0f)); //rotating y axis
            //cout << E->camera.camera_position.x << endl;
            //cout << E->camera.camera_position.y<< endl;
            //cout << E->camera.camera_position.z<< endl;
            break;
        
        case 'z':
            E->ZrotationAngle -= 0.05f;
            break;
        case 'Z':
            E->ZrotationAngle += 0.05f;
            break;
        case 'y':
            E->YrotationAngle -= 0.05f;
            break;
        case 'Y':
            E->YrotationAngle += 0.05f;
            break;
        case 'x':
            E->XrotationAngle -= 0.05f;
            break;
        case 'X':
            E->XrotationAngle += 0.05f;
            break;
         
        case 'w':
            E->camera.Move(FORWARD);
            break;
        case 'a':
            E->camera.Move(LEFT);
            //camera.SetLookAt(glm::vec3(0, 0, 0));
            break;
        case 's':
            E->camera.Move(BACK);
            break;
        case 'd':
            E->camera.Move(RIGHT);
            //camera.SetLookAt(glm::vec3(0, 0, 0));
            break;
        case 'q':
            E->camera.Move(DOWN);
            break;
        case 'e':
            E->camera.Move(UP);
            break;
        case 'r':
            E->camera.SetPosition(glm::vec3(2, 0, 0));
            E->camera.SetLookAt(glm::vec3(0, 0, 0));
            E->XrotationAngle = 0.0;
            E->YrotationAngle = 0.0;
            E->ZrotationAngle = 0.0;
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
	E->camera.SetPos(button, state, x, y);
}

//Used when person drags mouse around
void CallBackMotionFunc(int x, int y) {
	E->camera.Move2D(x, y);
}


void DisplayFunc() {
    
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, E->window.size.x, E->window.size.y);

	glm::mat4 model, view, projection;
	

    //model = glm::rotate(model, XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    //model = glm::rotate(model, YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    //model = glm::rotate(model, ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
    /*
    float radius = glm::distance(E->camera.camera_position, glm::vec3(0.0f,0.0f,0.0f));
    
    if(radius <= E->as.m_fInnerRadius+.005f){
        cout << "distance to centre = " << radius << endl;
        cout<<"pre inside : " << radius << endl;
        cout<<"  x: " <<E->camera.camera_position.x << endl;
        cout<<"  y: " <<E->camera.camera_position.y << endl;
        cout<<"  z: " <<E->camera.camera_position.z << endl;
        float   theta, phi;
        theta = acos(E->camera.camera_position.z/radius);
        phi = atan2(E->camera.camera_position.y,E->camera.camera_position.x);

        radius = E->as.m_fInnerRadius+.005f;
        E->camera.camera_position.x = radius*sin(theta)*cos(phi);
        E->camera.camera_position.y = radius*sin(theta)*sin(phi);
        E->camera.camera_position.z = radius*cos(theta);
        
        cout<<"post outside : " << radius << endl;
        cout<<"  x: " <<E->camera.camera_position.x << endl;
        cout<<"  y: " <<E->camera.camera_position.y << endl;
        cout<<"  z: " <<E->camera.camera_position.z << endl;
        
    }
    */
    
    E->camera.Update();
    
    E->camera.GetMatricies(projection, view, model);
    
	glm::mat4 mvp = projection * view * model;	//Compute the mvp matrix
	
    glLoadMatrixf(glm::value_ptr(mvp));
	
    // Enables Depth Testing
    glEnable(GL_DEPTH_TEST);
    
    // The Type Of Depth Testing To Do
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_MULTISAMPLE);
    
    
    
    float camera_magnitude = glm::length(E->camera.camera_position);
    float camera_magnitude_squared = pow(camera_magnitude ,2.0f);
    
    E->groundFromSpace.enable();
        E->groundFromSpace.SetUniform("model", model );
        E->groundFromSpace.SetUniform("view", view );
        E->groundFromSpace.SetUniform("projection", projection );
        E->groundFromSpace.SetUniform("v3CameraPos", E->camera.camera_position );
        E->groundFromSpace.SetUniform("v3LightPos", glm::normalize(E->camera.camera_position) );
        E->groundFromSpace.SetUniform("v3InvWavelength", E->as.m_fWavelength4_inv );
        E->groundFromSpace.SetUniform("fCameraHeight", camera_magnitude);
        E->groundFromSpace.SetUniform("fCameraHeight2", camera_magnitude_squared);
        E->groundFromSpace.SetUniform("fInnerRadius", E->as.m_fInnerRadius);
        E->groundFromSpace.SetUniform("fInnerRadius2", E->as.m_fInnerRadius2);
        E->groundFromSpace.SetUniform("fOuterRadius", E->as.m_fOuterRadius);
        E->groundFromSpace.SetUniform("fOuterRadius2", E->as.m_fOuterRadius2);
        E->groundFromSpace.SetUniform("fKrESun", E->as.m_Kr*E->as.m_ESun);
        E->groundFromSpace.SetUniform("fKmESun", E->as.m_Km*E->as.m_ESun);
        E->groundFromSpace.SetUniform("fKr4PI", E->as.m_Kr4PI);
        E->groundFromSpace.SetUniform("fKm4PI", E->as.m_Km4PI);
        E->groundFromSpace.SetUniform("fScale", E->as.m_fScale);
        E->groundFromSpace.SetUniform("fScaleDepth", E->as.m_fRayleighScaleDepth);
        E->groundFromSpace.SetUniform("fScaleOverScaleDepth", E->as.m_fScaleOverScaleDepth);
        E->groundFromSpace.SetUniform("g", E->as.m_g);
        E->groundFromSpace.SetUniform("g2", E->as.m_g2);
        E->groundFromSpace.SetUniform("nSamples", E->as.m_nSamples);
        E->groundFromSpace.SetUniform("fSamples", (float)E->as.m_nSamples);
    
        E->ground.draw();
    
    E->groundFromSpace.disable();
    
    
    E->skyFromSpace.enable();
        E->skyFromSpace.SetUniform("model", model );
        E->skyFromSpace.SetUniform("view", view );
        E->skyFromSpace.SetUniform("projection", projection );
        E->skyFromSpace.SetUniform("v3CameraPos", E->camera.camera_position );
        E->skyFromSpace.SetUniform("v3LightPos", glm::normalize(E->camera.camera_position) );
        E->skyFromSpace.SetUniform("v3InvWavelength", E->as.m_fWavelength4_inv );
        E->skyFromSpace.SetUniform("fCameraHeight", camera_magnitude);
        E->skyFromSpace.SetUniform("fCameraHeight2", camera_magnitude_squared);
        E->skyFromSpace.SetUniform("fInnerRadius", E->as.m_fInnerRadius);
        E->skyFromSpace.SetUniform("fInnerRadius2", E->as.m_fInnerRadius2);
        E->skyFromSpace.SetUniform("fOuterRadius", E->as.m_fOuterRadius);
        E->skyFromSpace.SetUniform("fOuterRadius2", E->as.m_fOuterRadius2);
        E->skyFromSpace.SetUniform("fKrESun", E->as.m_Kr*E->as.m_ESun);
        E->skyFromSpace.SetUniform("fKmESun", E->as.m_Km*E->as.m_ESun);
        E->skyFromSpace.SetUniform("fKr4PI", E->as.m_Kr4PI);
        E->skyFromSpace.SetUniform("fKm4PI", E->as.m_Km4PI);
        E->skyFromSpace.SetUniform("fScale", E->as.m_fScale);
        E->skyFromSpace.SetUniform("fScaleDepth", E->as.m_fRayleighScaleDepth);
        E->skyFromSpace.SetUniform("fScaleOverScaleDepth", E->as.m_fScaleOverScaleDepth);
        E->skyFromSpace.SetUniform("g", E->as.m_g);
        E->skyFromSpace.SetUniform("g2", E->as.m_g2);
        E->skyFromSpace.SetUniform("nSamples", E->as.m_nSamples);
        E->skyFromSpace.SetUniform("fSamples", (float)E->as.m_nSamples);
    
        glFrontFace(GL_CW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
        E->sky.draw();
    
        glDisable(GL_BLEND);
        glFrontFace(GL_CCW);
    
    E->skyFromSpace.disable();
    
    
    /*
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    Sphere::drawSphere(64, glm::vec3(0.0f, 0.0f, 0.0f), m_fInnerRadius);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    */
    
    
     /*
    // texture map sphere
    // bind the texture and set the "tex" uniform in the fragment shader
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, E->continents.texture_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, E->field.texture_id);
    
    // perform scene roations
    model = glm::rotate(model, E->XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    model = glm::rotate(model, E->YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    model = glm::rotate(model, E->ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
   
    E->texMap.enable();
    
        E->texMap.SetUniform("Texture0", 0); //set to 0 because the texture is bound to GL_TEXTURE0
        E->texMap.SetUniform("Texture1", 1);
        E->texMap.SetUniform("model", model );
        E->texMap.SetUniform("view", view );
        E->texMap.SetUniform("projection", projection );
    
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
        E->texSphere.draw();
    
        glDisable(GL_BLEND);
    E->texMap.disable();
    
    glDisable(GL_BLEND);
    glDisable( GL_TEXTURE_2D );
    //glDisable( GL_TEXTURE_RECTANGLE_ARB );
    */
    
    // perform scene roations
    //model = glm::rotate(model, E->XrotationAngle, glm::vec3(1,0,0));//rotating x axis
    //model = glm::rotate(model, E->YrotationAngle, glm::vec3(0,1,0));//rotating y axis
    //model = glm::rotate(model, E->ZrotationAngle, glm::vec3(0,0,1));//rotating z axis
    
    E->passThrough.enable();
        E->passThrough.SetUniform("model", model );
        E->passThrough.SetUniform("view", view );
        E->passThrough.SetUniform("projection", projection );
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            E->bathy_mesh.draw();
        glDisable(GL_BLEND);
            //E->test_sphere.draw();
        //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    E->passThrough.disable();
    
    
    glutSwapBuffers();
}

//Redraw based on fps set for window
void TimerFunc(int value) {
	if (E->window.window_handle != -1) {
		glutTimerFunc(E->window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}



int main(int argc, char **argv) {
    
    E = new slicer;
    
    //glut boilerplate
    glutInit(&argc, argv);
    glutInitWindowSize(1024, 512);
    glutInitWindowPosition(0, 0);
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
#endif
    //Setup window and callbacks
    E->window.window_handle = glutCreateWindow("slicer");
    glutReshapeFunc(ReshapeFunc);
    glutDisplayFunc(DisplayFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);
    glutMouseFunc(CallBackMouseFunc);
    glutMotionFunc(CallBackMotionFunc);
    glutPassiveMotionFunc(CallBackPassiveFunc);
    glutTimerFunc(E->window.interval, TimerFunc, 0);

    // init OpenGL
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "GLEW failed to initialize." << endl;
        exit(1);
    }
    if (!glewIsSupported("GL_VERSION_3_2")){
        cerr << "OpenGL 3.2 not supported." << endl;
        exit(1);
    }

    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    
    
    // init the app
    
    E->init();
    
    //Start the glut loop!
    glutMainLoop();

    return 0;
}