#include <iostream>
#include <cstdlib>
#include "../external/GL/glew.h"
#include "../external/GL/freeglut.h"
#include <fstream>
#include "../external/Eigen/Eigen"
#include "../include/Geometry.h"
#include "../include/glslShader.h"
#include "../include/camera.h"
#include "../include/picking.h"
#include "../external/IMGUI/imgui.h"
#include "../external/IMGUI/imgui_impl_glut.h"
#define BUFFER_LENGTH 64
static Camera camera;
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
static unsigned int width = 1260;
static unsigned int height = 760;
static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static int lastX = 0, lastY = 0, lastZoom = 0;

static bool fullScreen = false;

//static bool wireframe  = false;
//static bool meshgrid   = true;
//static bool normal_v   = false;
//static bool gouraud	   = false;

static Eigen::Vector3d mouse_pose;
static Eigen::Vector3d mouse_move;

static float inner = 10;
static float outer = 10;
static bool show_line = false;
std::vector<Geometry*> hw1;
Geometry* geo;
Geometry* geo2;
Geometry* geo3;
Geometry* geo4;
glslShader* shader;
glslShader* line_shader;
glslShader* sphere_shader;
static Picking pick;






void draw_geovector(Geometry* geovector){
		float *pj_mat = new float[16];
		float *v_mat = new float[16];
		float normal_mat[9];
		glPushMatrix();
		glTranslated(double(geovector->orientation(3,0)),double(geovector->orientation(3,1)),double(geovector->orientation(3,2)));
		glColor4f(0.f, 0.f, 1.f,1.f);
		glGetFloatv(GL_PROJECTION_MATRIX, pj_mat);
		glGetFloatv(GL_MODELVIEW_MATRIX, v_mat);
		normal_mat[0] = v_mat[0]; normal_mat[1] = v_mat[4]; normal_mat[2] = v_mat[8];
		normal_mat[3] = v_mat[1]; normal_mat[4] = v_mat[5]; normal_mat[5] = v_mat[9];
		normal_mat[6] = v_mat[2]; normal_mat[7] = v_mat[6]; normal_mat[8] = v_mat[10];
		sphere_shader->enable();
		sphere_shader->setUniformMatrix4fv("view_mat", v_mat, false);
		sphere_shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
		sphere_shader->setUniformMatrix3fv("NormalMatrix", normal_mat, false);
		sphere_shader->setUniform3f("light_pos", 0, 100, 100);
		sphere_shader->setUniform3f("DiffuseMaterial", 1, 1, 0);
		sphere_shader->setUniform3f("AmbientMaterial", 0.1, 0, 0);

		sphere_shader->setUniform1f("TessLevelOuter", outer);
		sphere_shader->setUniform1f("TessLevelInner", inner);
		if(geovector->hit ==true) {
			sphere_shader->setUniform1i("hit",0);
		} else{
			sphere_shader->setUniform1i("hit",1);
		}
		geovector->Render();
		sphere_shader->disable();
		glPopMatrix();
		delete[] v_mat;
		delete[] pj_mat;



}








void drawGUI()
{

	GLdouble *pj_mat = new GLdouble[16];
	GLdouble *v_mat = new GLdouble[16];
	GLint *identity = new GLint[4];
	GLdouble xx,yy,zz;
	glGetDoublev(GL_PROJECTION_MATRIX, pj_mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, v_mat);
	glGetIntegerv(GL_VIEWPORT,identity);
	Eigen::Matrix4d projection = GL2Eigen(pj_mat);
	Eigen::Matrix4d modelview = GL2Eigen(v_mat);
	if(pick.pick_button){
		pick.modelview_mat = modelview;
		pick.projection_mat = projection;
		pick.pick_cal();
		pick.hit_check();
	}
    ImGui_ImplGLUT_NewFrame(width, height);
	ImGui::PushID("mouse pose");
	std::string hit1 = "no";std::string hit2 = "no";std::string hit3 = "no";
	if(geo->hit)hit1 = "HIT";
	if(geo2->hit)hit2 = "HIT";
	if(geo3->hit)hit3 = "HIT";
	ImGui::Text("SPHERE 1 %s",hit1.c_str());
	ImGui::Text("SPHERE 2 %s",hit2.c_str());
	ImGui::Text("SPHERE 3 %s",hit3.c_str());

	ImGui::Text("\n\nmouse screen pos =  x: %f , y: %f",mouse_pose(0),mouse_pose(1));
	ImGui::Text("near_ pos =  x: %f , y: %f , z: %f",pick.near_(0),pick.near_(1),pick.near_(2));
	ImGui::Text("far_ pos =  x: %f , y: %f , z: %f",pick.far_(0),pick.far_(1),pick.far_(2));


    ImGui::Text("\n\nSPHERE1 pos : %f,%f,%f",geo->orientation(3,0),geo->orientation(3,1),geo->orientation(3,2));
    ImGui::Text("SPHERE1 pos : %f,%f,%f",geo2->orientation(3,0),geo2->orientation(3,1),geo2->orientation(3,2));
    ImGui::Text("SPHERE1 pos : %f,%f,%f",geo3->orientation(3,0),geo3->orientation(3,1),geo3->orientation(3,2));

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
    case '1':
		inner++;
		break;
    case '2':
		outer++;
    	break;
	case '9':
		if (inner > 1) {
			inner--;
		    ImGui_ImplGLUT_NewFrame(width, height);
			ImGui::PushID("mouse pose");
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

        pick.pick_button = false;
        pick.hit_update();
        mouse_move = Eigen::Vector3d(0,0,0);

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
        } else if (button == GLUT_LEFT_BUTTON){
        	mouse_pose = Eigen::Vector3d(x,y,0);
        	pick.get_mouse(Eigen::Vector2d(width,height),mouse_pose);
            pick.pick_button = true;
            pick.hit_update();
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
    } else if (pick.pick_button == true){
		mouse_move = Eigen::Vector3d(x-mouse_pose(0),mouse_pose(1)-y,0);
		mouse_move(0) = mouse_move(0)/width*2;
		mouse_move(1) = mouse_move(1)/height*2;
		mouse_pose(0) = x;	mouse_pose(1) = y;

    }
    ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	glutPostRedisplay();
}

void idle() {
}
void display() {
	float *pj_mat = new float[16];
	float *v_mat = new float[16];
	float normal_mat[9];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if (show_line) {
		glDisable(GL_DEPTH_TEST);
	}
	else {
		glEnable(GL_DEPTH_TEST);
	}
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1260.0/760.0, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 10, 0);
	gluLookAt(0,10,-10,0,0,0,0,0,1);
	camera.apply();

//	glPushMatrix();
//	glTranslated(geo->orientation(3,0),geo->orientation(3,1),geo->orientation(3,2));
//	glColor4f(0.f, 0.f, 1.f,1.f);
//	glGetFloatv(GL_PROJECTION_MATRIX, pj_mat);
//	glGetFloatv(GL_MODELVIEW_MATRIX, v_mat);
//	normal_mat[0] = v_mat[0]; normal_mat[1] = v_mat[4]; normal_mat[2] = v_mat[8];
//	normal_mat[3] = v_mat[1]; normal_mat[4] = v_mat[5]; normal_mat[5] = v_mat[9];
//	normal_mat[6] = v_mat[2]; normal_mat[7] = v_mat[6]; normal_mat[8] = v_mat[10];
//	sphere_shader->enable();
//	sphere_shader->setUniformMatrix4fv("view_mat", v_mat, false);
//	sphere_shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
//	sphere_shader->setUniformMatrix3fv("NormalMatrix", normal_mat, false);
//	sphere_shader->setUniform3f("light_pos", 0, 100, 100);
//	sphere_shader->setUniform3f("DiffuseMaterial", 1, 1, 0);
//	sphere_shader->setUniform3f("AmbientMaterial", 0.1, 0, 0);
//
//	sphere_shader->setUniform1f("TessLevelOuter", outer);
//	sphere_shader->setUniform1f("TessLevelInner", inner);
//	if(geo->hit ==true) {
//		sphere_shader->setUniform1i("hit",0);
//	} else
//		sphere_shader->setUniform1i("hit",1);
//	geo->Render();
//	sphere_shader->disable();
//	glPopMatrix();
	draw_geovector(geo);

	draw_geovector(geo2);
	draw_geovector(geo3);
	pick.move_object(mouse_move);
	pick.get_mouse(Eigen::Vector2d(width,height),mouse_pose);
	pick.hit_update();

	drawGUI();

	glutSwapBuffers();

	delete[] v_mat;
	delete[] pj_mat;
}
void init()
{
    glClearColor(0.447f, 0.565f, 0.604f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGLUT_Init();
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
	// camera.resize(width, height);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		const GLubyte *e = glewGetErrorString(err);
		//printf("Error: %s\n", e);
	}
	shader = new glslShader();
	line_shader = new glslShader();
	sphere_shader = new glslShader();
	//shader->loadFromFile("shaders.vs", "shaders.fs");
	const char* vs_shader  = 	"./shader/phong_shader/shaders.vs";		const char* sphere_shader_vs	=	"./shader/sphere_shader/tes_shader.vs";
	const char* tcs_shader = 	"./shader/phong_shader/shaders.tcs";	const char* sphere_shader_tcs	=	"./shader/sphere_shader/tes_shader.tcs";
	const char* tes_shader =	"./shader/phong_shader/shaders.tes";	const char* sphere_shader_tes	=	"./shader/sphere_shader/tes_shader.tes";
	const char* gs_shader  = 	"./shader/phong_shader/shaders.gs";		const char* sphere_shader_gs	=	"./shader/sphere_shader/tes_shader.gs";
	const char* fs_shader  = 	"./shader/phong_shader/shaders.fs";		const char* sphere_shader_fs	=	"./shader/sphere_shader/tes_shader.fs";
	const char* line_gs_shader  = 	"./shader/line_shader/line_shader.gs";
	const char* line_fs_shader  = 	"./shader/line_shader/line_shader.fs";


	shader->loadFromFile_FULL(vs_shader, tcs_shader, tes_shader, gs_shader, fs_shader);
	line_shader->loadFromFile_FULL_LINE(vs_shader, tcs_shader, tes_shader, line_gs_shader,line_fs_shader);
	sphere_shader->loadFromFile_FULL(sphere_shader_vs, sphere_shader_tcs, sphere_shader_tes, sphere_shader_gs, sphere_shader_fs);
	geo = new Geometry();
	geo->set_sphere(1,-5,0,0);
	geo->setBuffer();

	geo2 = new Geometry();
	geo2->set_sphere(1,0,0,-10);
	geo2->setBuffer();

	geo3 = new Geometry();
	geo3->set_sphere(1,5,0,-20);
	geo3->setBuffer();



	pick.item.push_back(geo);	 pick.item.push_back(geo2);	pick.item.push_back(geo3);
	//hw1.push_back(geo);			 hw1.push_back(geo4);		hw1.push_back(geo3);
	//geo->loadObj("cylinder_non_smoothing.obj");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
    init();
	glutMainLoop();
	

	return EXIT_SUCCESS;
}




