//#include <windows.h>
#include <iostream>
#include <gl/gl.h>
#include <gl/glut.h>
#include <ctime>
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

using namespace std;


void reshape(int w, int h) { camera.resize(w, h); }

void display() {
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    glTranslatef(-offset_pos[0]/10,-offset_pos[1]/10,-offset_pos[2]/10);
    //glTranslatef(-10,-10,-10);
    glLineWidth(1);
    glColor3f(1,0,0);
    glBegin(GL_LINE_STRIP);
    {
    	glVertex3f(0,0,0);
    	glVertex3f(100,0,0);
    }
    glColor3f(0,1,0);
    glEnd();
    glBegin(GL_LINE_STRIP);
	{
		glVertex3f(0,0,0);
		glVertex3f(0,100,0);
    }
	glEnd();
	glColor3f(1,1,0);
	glBegin(GL_LINE_STRIP);
	{
		glVertex3f(0,0,0);
		glVertex3f(0,0,100);
	}

    glEnd();
    glPushMatrix();
    glColor3f(0,1,0);
    glScalef(0.1,0.1,0.1);
    t_pos = pos_d;
    target->target_global_pos = t_pos;
    glTranslatef(t_pos.x,t_pos.y,t_pos.z);
    glutWireCube(10);
    glPopMatrix();

    drawTriangle();

    //drawGUI
    glFlush();
    glutSwapBuffers();
    if(stop == 1){
    	step++;
    	if(step > frame_num-1)
    		step = 0;
    	Body.update_by_bvh(int(step),&Body.root);
    }
    else if(stop == 2){
    	if(t_later_sum > CLOCKS_PER_SEC*R.frame_time/10){
    		t_later_sum =clock_t(0);
    		step++;
    	}
    	if(step > frame_num-1)
    	    step = 0;
    	Body.update_by_bvh(int(step),&Body.root);
    }
    else if(stop == 3){
    	Body.update_draw_IK(&Body.root,target);
    }
    else{

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
    case 'u':
		break;
    case 'p':
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

void idle() {
}

int main(int argc, char** argv) {

	

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
