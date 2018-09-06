
#include "../external/GL/glew.h"
#include "../external/GL/freeglut.h"
#include "../external/Eigen/Eigen"
#include "../include/camera.h"
#include "../external/IMGUI/imgui.h"
#include "../external/IMGUI/imgui_impl_glut.h"
#include "../include/picking.h"


static Picking pick;

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

static Eigen::Vector3d mouse_pose;
static Eigen::Vector3d mouse_move;

static bool show_line = false;

float colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
float colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };

float glassAmbient[4] = {0.1000 ,0.1000 ,0.1000};
float glassDiffuse[4] = {0.5020 ,0.5020, 0.5020 };
float glassSpecular[4] = { 1.0000, 1.0000 ,1.0000  };

float goldAmb[4] = {0.5265, 0.2735 ,0.0122};
float goldDiff[4] = {1.0000 ,0.5184, 0.0286};
float goldSpec[4] = {0.3000 ,0.3000, 0.3000};


GLfloat Light_pos_0[] = { 50.0f, 120.0f, 30.0, 1 };
GLfloat Light_pos_1[] = { 100.0f, 120.0f, 30.0, 1 };
GLfloat Light_pos_2[] = { 100.0f, 120.0f, 30.0, 0 };


GLfloat lightDiffuse[]  = { 0.50, 0.5, 0.5, 1.0};
GLfloat lightSpecular[] = { 0.90, 0.90, 0.90, 1.0};
GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};




void reshape(int w, int h) { camera.resize(w, h);  }

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
