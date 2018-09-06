#include <iostream>
#include <cstdlib>
#include <fstream>

#include "../external/GL/glew.h"
#include "../external/GL/freeglut.h"
#include "../external/Eigen/Eigen"
#include "../external/IMGUI/imgui.h"
#include "../external/IMGUI/imgui_impl_glut.h"


#include "../include/camera.h"


#define BUFFER_LENGTH 64
static Camera camera;
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
static unsigned int width = 1260;
static unsigned int height = 760;

static unsigned int tex_width = 512;
static unsigned int tex_height = 512;
static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static int lastX = 0, lastY = 0, lastZoom = 0;
GLuint fbo_id;
GLuint depth_id;
GLuint texture_id;
GLuint texture_rgb_id;
static float ani_time =0;



static bool fullScreen = false;

using namespace std;

void  initialize_render2texture(){
	std::cout << "Initializing FBO" << "\n";
	// depth buffer는 바인딩 완료.
	// 이제 textrue 추가하자.
	glGenTextures(1,&texture_rgb_id);
	glBindTexture(GL_TEXTURE_2D,texture_rgb_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,tex_width,tex_height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);  //실질적으로 texture가 있는것은 아니므로 NULL 바인딩.
    glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1,&texture_id);
	glBindTexture(GL_TEXTURE_2D,texture_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,tex_width,tex_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);  //실질적으로 texture가 있는것은 아니므로 NULL 바인딩.
    glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffersEXT(1,&fbo_id);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fbo_id); // fbo binding : 이제 부터 이 fbo에 할당됨.

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,texture_id,0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,texture_rgb_id,0);
	//일단 색상버퍼에다가 texture를 바인딩해줌.

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//setup 자체는 끝낫고 이제 텍스쳐로 렌더링을 하자.
	std::cout << "Initializing FBO complete" << "\n";

}

void drawGUI()
{
    ImGui_ImplGLUT_NewFrame(width, height);
	ImGui::PushID("mouse pose");
	
    ImGui::Text("\n\nCONTROLL KEY : \nMouse right drag \nCtl + mouse right drag\nShift + mouse right drag");
    ImGui::Text("\nMouse Left click IS PICKING BUTTON");


    ImGui::PopID();
    ImGui::Render();
}

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
    
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed   = false;
        mouseRotatePressed = false;
        mouseZoomPressed   = false;
       
    } else {
        if (button==GLUT_RIGHT_BUTTON && GLUT_ACTIVE_SHIFT==glutGetModifiers()) {
            lastX = x;
            lastY = y;
            mouseMovePressed   = true;
            mouseRotatePressed = false;
            mouseZoomPressed   = false;
        } else if (button==GLUT_RIGHT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers()) {
            lastZoom = y;
            mouseMovePressed   = false;
            mouseRotatePressed = false;
            mouseZoomPressed   = true;
        } else if (button==GLUT_RIGHT_BUTTON) {	gluLookAt(0,10,-10,0,0,0,0,0,1);

            camera.beginRotate(x, y);
            mouseMovePressed   = false;
            mouseRotatePressed = true;
            mouseZoomPressed   = false;
        } 

    }
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)x, (float)y);
	if (state == GLUT_DOWN && (button == GLUT_LEFT_BUTTON))
	   io.MouseDown[0] = true;
   else
	   io.MouseDown[0] = false;

   if (state == GLUT_DOWN && (button == GLUT_RIGHT_BUTTON))
	   io.MouseDown[1] = true;
   else
	   io.MouseDown[1] = false;
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
    ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	glutPostRedisplay();
}

void draw_rtt(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fbo_id);  // fbo에다가 그려준다
	glClearColor(0.2,0.3,0.1,0.5f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,tex_width,tex_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, float(tex_width)/float(tex_height), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -3);
	Eigen::Array3d v(ani_time*0.1,ani_time,ani_time);
	float sin_value = v.sin()(0);
	glTranslatef(0,0,sin_value*0.3);
	glRotated(v(0)*5,1,0,0);
	glColor3f(1,1,0);
	glutSolidTeapot(0.5f);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);  // 언바인딩 해준다.


	glClearColor(0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, float(width)/float(height), 0.1, 10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.apply();
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,texture_id); //fbo에 그려진 텍스쳐를 쓸거다.
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glRotated(v(0)*6,1,1,0);
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);//front
		glTexCoord2f(0,0); glVertex3f(-1.1,-1.1,1.1);
		glTexCoord2f(1,0); glVertex3f(1.1,-1.1,1.1);
		glTexCoord2f(1,1); glVertex3f(1.1,1.1,1.1);
		glTexCoord2f(0,1); glVertex3f(-1.1,1.1,1.1);

		glNormal3f(1,0,0);//right
		glTexCoord2f(0,0); glVertex3f(1.1,-1.1,1.1);
		glTexCoord2f(1,0); glVertex3f(1.1,-1.1,-1.1);
		glTexCoord2f(1,1); glVertex3f(1.1,1.1,-1.1);
		glTexCoord2f(0,1); glVertex3f(1.1,1.1,1.1);
		
		glNormal3f(0,1,0);//top
		glTexCoord2f(0,0); glVertex3f(-1.1,1.1,1.1);
		glTexCoord2f(1,0); glVertex3f(1.1,1.1,1.1);
		glTexCoord2f(1,1); glVertex3f(1.1,1.1,-1.1);
		glTexCoord2f(0,1); glVertex3f(-1.1,1.1,-1.1);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0); 


	//color texture
	glBindTexture(GL_TEXTURE_2D,texture_rgb_id); //fbo에 그려진 텍스쳐를 쓸거다.

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glNormal3f(0,0,-1);//back
		glTexCoord2f(1,0); glVertex3f(-1.1,-1.1,-1.1);
		glTexCoord2f(0,0); glVertex3f(1.1,-1.1,-1.1);
		glTexCoord2f(0,1); glVertex3f(1.1,1.1,-1.1);
		glTexCoord2f(1,1); glVertex3f(-1.1,1.1,-1.1);
		
		glNormal3f(-1,0,0);	//left
		glTexCoord2f(0,0); glVertex3f(-1.1,-1.1,-1.1);
		glTexCoord2f(1,0); glVertex3f(-1.1,-1.1,1.1);
		glTexCoord2f(1,1); glVertex3f(-1.1,1.1,1.1);
		glTexCoord2f(0,1); glVertex3f(-1.1,1.1,-1.1);

		glNormal3f(0,-1,0);	//bottom
		glTexCoord2f(0,0); glVertex3f(-1.1,-1.1,-1.1);
		glTexCoord2f(1,0); glVertex3f(1.1,-1.1,-1.1);
		glTexCoord2f(1,1); glVertex3f(1.1,-1.1,1.1);
		glTexCoord2f(0,1); glVertex3f(-1.1,-1.1,1.1);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0); 

	drawGUI();
	glutSwapBuffers();


}

void init()
{
    glClearColor(0.447f, 0.565f, 0.604f, 1.0f); 
    ImGui_ImplGLUT_Init();
	initialize_render2texture();

	glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glColor3f(0.5, 0.5, 0.5);
    GLfloat lightPosition[] = { 0.0f, 100.0f, 100.0, 0.0f };
    GLfloat lightDiffuse[]  = { 1.00, 0.99, 0.98, 1.0};
    GLfloat lightSpecular[] = { 0.10, 0.10, 0.10, 1.0};
    GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);

	
}

void animation(int i){
	ani_time+= i;
	draw_rtt();
	glutTimerFunc(10,animation,1);

	return;
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Trackball Demo");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	camera.resize(width, height);
	glClearColor(0.0, 0.1, 0.3, 1.0);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		const GLubyte *e = glewGetErrorString(err);
		//printf("Error: %s\n", e);
	}
	glutTimerFunc(10,animation,1);
	glutDisplayFunc(draw_rtt);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
    init();
	glutMainLoop();
	

	return EXIT_SUCCESS;
}




