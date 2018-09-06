#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Geometry.h>
#include <glslShader.h>
#include "camera.h"
static Camera camera;

static unsigned int width = 1280;
static unsigned int height = 720;

static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static int lastX = 0, lastY = 0, lastZoom = 0;

static bool fullScreen = false;
//static bool wireframe  = false;
//static bool meshgrid   = true;
//static bool normal_v   = false;
//static bool gouraud	   = false;
static float inner = 1;
static float outer = 1;
static bool show_line = false;
Geometry* geo;
glslShader* shader;
glslShader* line_shader;
void reshape(int w, int h) { camera.resize(w, h); }

void keyboardCB(unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow(width, height);
            fullScreen = false;
        } else {
            glutFullScreen();
            fullScreen = true;
        }
        break;
    case '1':
		inner++;
		break;
    case '2':
		outer++;
    	break;
	case '9':
		if (inner > 1) {
			inner--;
		}
		break;
	case '0':
		if (outer > 1) {
			outer--;
		}
		break;
	case 'o':
		show_line = !show_line;
		break;
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed   = false;
        mouseRotatePressed = false;
        mouseZoomPressed   = false;
    } else {
        if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_SHIFT==glutGetModifiers()) {
            lastX = x;
            lastY = y;
            mouseMovePressed   = true;
            mouseRotatePressed = false;
            mouseZoomPressed   = false;
        } else if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers()) {
            lastZoom = y;
            mouseMovePressed   = false;
            mouseRotatePressed = false;
            mouseZoomPressed   = true;
        } else if (button==GLUT_LEFT_BUTTON) {
            camera.beginRotate(x, y);
            mouseMovePressed   = false;
            mouseRotatePressed = true;
            mouseZoomPressed   = false;
        }
    }
    glutPostRedisplay();
}

void motionCB(int x, int y) {
    if (mouseRotatePressed == true)
        camera.rotate(x, y);
    else if (mouseMovePressed == true) {
        camera.move((x - lastX) / static_cast<float>(width),
                    (lastY - y) / static_cast<float>(height), 0.0);
        lastX = x;
        lastY = y;
    } else if (mouseZoomPressed == true) {
        camera.zoom(float(y - lastZoom) / height);
        lastZoom = y;
    }
    glutPostRedisplay();
}

void idle() {
}
void display() {
	float *pj_mat = new float[16];
	float *v_mat = new float[16];
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if (show_line) {
		glDisable(GL_DEPTH_TEST);
	}
	else {
		glEnable(GL_DEPTH_TEST);
	}
	
	glViewport(0,0,1000,1000);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -100);
	glRotatef(30.f, 1.f, 0.f, 0.f);
	camera.apply();
	glColor4f(0.f, 0.f, 1.f,1.f);

	glGetFloatv(GL_PROJECTION_MATRIX, pj_mat);
	glGetFloatv(GL_MODELVIEW_MATRIX, v_mat);
	shader->enable();
	
	shader->setUniformMatrix4fv("view_mat", v_mat, false);
	shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
	

	shader->setUniform3f("light_pos", 0, 100, 100);
	shader->setUniform1f("TessLevelOuter", outer);
	shader->setUniform1f("TessLevelInner", inner);
	

	geo->Render();
	shader->disable();
	if (show_line) {
		line_shader->enable();
		line_shader->setUniformMatrix4fv("view_mat", v_mat, false);
		line_shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
		line_shader->setUniform3f("light_pos", 0, 100, 100);
		line_shader->setUniform1f("TessLevelOuter", outer);
		line_shader->setUniform1f("TessLevelInner", inner);
		geo->Render();
		line_shader->disable();
	}
	glutSwapBuffers();

	delete[] v_mat;
	delete[] pj_mat;
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("");
	// camera.resize(width, height);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		const GLubyte *e = glewGetErrorString(err);
		printf("Error: %s\n", e);
	}
	shader = new glslShader();
	line_shader = new glslShader();
	//shader->loadFromFile("shaders.vs", "shaders.fs");
	shader->loadFromFile_FULL("shaders.vs", "shaders.tcs", "shaders.tes", "shaders.gs", "shaders.fs");
	line_shader->loadFromFile_FULL_LINE("shaders.vs", "shaders.tcs", "shaders.tes", "line_shader.gs", "line_shader.fs");
	geo = new Geometry();
	geo->loadObj("cylinder_non_smoothing.obj");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
	glutMainLoop();
	

	return EXIT_SUCCESS;
}


