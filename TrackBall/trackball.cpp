#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include "Body_structure.h"
#include "camera.h"

static Camera camera;

static unsigned int width = 700;
static unsigned int height = 700;

static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static int lastX = 0, lastY = 0, lastZoom = 0;

static bool fullScreen = false;
static bool wireframe  = false;
static bool meshgrid   = true;
static bool normal_v   = false;
static bool gouraud	   = false;
Body_structure Body;
float step;
int stop;
void drawTriangle() {
// Draw something Here
	//std::cout<<"step = "<<step<<std::endl;
	Body.root.draw_node();
}

void reshape(int w, int h) { camera.resize(w, h); }

void display() {
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    drawTriangle();
    glFlush();
    glutSwapBuffers();
    if(stop != 0){
		step += (float(stop)/10);
		if(step >= 3600){
			step =0;
		}
		Body.update_angle(step);
    }
    else{
    	step=0;
    	Body.update_angle(step);
    }
}

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
    	stop = 1;
    	break;
    case '2':
		stop = 2;
		break;
    case '3':
		stop = 3;
		break;
    case 's':
    	stop = 0;
    	break;
    case 'q':
        exit(0);
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

void idle() { glutPostRedisplay(); }

int main(int argc, char** argv) {
	stop = 0;
	Body.generate_Luis_body();
	step = 0;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("Trackball Demo");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    camera.resize(width, height);
    glClearColor(0.0, 0.1, 0.3, 1.0);

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);

    glutMainLoop();
    return 0;
}
