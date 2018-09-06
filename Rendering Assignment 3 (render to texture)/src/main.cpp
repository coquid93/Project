#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "../include/main_etc.h"
#include "../include/Geometry.h"
#include "../include/glslShader.h"
#include "../include/render2texture.h"

std::vector<Geometry*> hw1;
Geometry* geo;
Geometry* tablelamp;
Geometry* tray;
Geometry* light0;
Geometry* light1;
Geometry* light2;
glslShader* shadow_shader;
glslShader* sphere_shader;

float colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
float colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };

GLfloat Light_pos_0[] = { 50.0f, 120.0f, 30.0, 1 };
GLfloat Light_pos_1[] = { 100.0f, 120.0f, 30.0, 1 };
GLfloat Light_pos_2[] = { 100.0f, 120.0f, 30.0, 0 };

GLfloat lightDiffuse[]  = { 0.50, 0.5, 0.5, 1.0};
GLfloat lightSpecular[] = { 0.90, 0.90, 0.90, 1.0};
GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};

render2tex *depth_tex0;
render2tex *depth_tex1;
render2tex *depth_tex2;

static int current_map = 2;


string IntToString (int a)
{
        ostringstream temp;
        temp<<a;
        return temp.str();
}
std::vector<Geometry*> geo_item;
void drawGUI();
void plain();
void inv_trans(float *input, float *output);
void draw_light();
void draw_shadow_geovector(Geometry* geovector){
		float *pj_mat = new float[16];
		float *model_mat = new float[16];
		float *view_mat = new float[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, view_mat);
		glGetFloatv(GL_PROJECTION_MATRIX, pj_mat);
        glPushMatrix();
        glLoadIdentity();
        glScalef(geovector->scale_factor, geovector->scale_factor, geovector->scale_factor);
		glTranslated(double(geovector->orientation(3,0)),double(geovector->orientation(3,1)),double(geovector->orientation(3,2)));
        glGetFloatv(GL_MODELVIEW_MATRIX, model_mat);
		shadow_shader->enable();
		shadow_shader->setUniformMatrix4fv("model_mat", model_mat, false);
		shadow_shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
		shadow_shader->setUniformMatrix4fv("view_mat", view_mat, false);
		geovector->Render_shadow();
		shadow_shader->disable();
		glPopMatrix();
		delete[] model_mat;
        delete [] pj_mat;
        delete [] view_mat;

}
void set_light_uniform(glslShader * shader , render2tex * shadow_map , int &index){
        string light_pos , light_view_mat , light_proj_mat , map_texture , turn ;
        string ind = IntToString(index);
        light_pos = "light[" + ind +"].position";
        light_view_mat = "light[" + ind +"].view_mat";
        light_proj_mat = "light[" + ind +"].proj_mat";
        map_texture = "light[" + ind +"].shadow_map";
        turn = "light[" + ind +"].turn";
        shader->setUniform4f(light_pos.c_str(), shadow_map->light->orientation(3,0), shadow_map->light->orientation(3,1), shadow_map->light->orientation(3,2),1.0 );
		shader->setUniformMatrix4fv(light_view_mat.c_str(), shadow_map->view_mat , false );
		shader->setUniformMatrix4fv(light_proj_mat.c_str(), shadow_map->proj_mat , false );
		shader->setUniform1i(map_texture.c_str(), index);
		shader->setUniform1i(turn.c_str(), shadow_map->light_switch);
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, shadow_map->texture_rgb_id);
        index++;
}
void draw_geovector(Geometry* geovector){
		float *pj_mat = new float[16];
		float *model_mat = new float[16];
		float *view_mat = new float[16];
		float *normal_mat = new float[9];
        int light_index = 0;
		glGetFloatv(GL_MODELVIEW_MATRIX, view_mat);
		glGetFloatv(GL_PROJECTION_MATRIX, pj_mat);
        inv_trans(view_mat , normal_mat);

        glPushMatrix();
        glLoadIdentity();
        glScalef(geovector->scale_factor ,geovector->scale_factor , geovector->scale_factor);
		glTranslated(double(geovector->orientation(3,0)),double(geovector->orientation(3,1)),double(geovector->orientation(3,2)));
        glGetFloatv(GL_MODELVIEW_MATRIX, model_mat);
        sphere_shader->enable();
		sphere_shader->setUniformMatrix4fv("model_mat", model_mat, false);
		sphere_shader->setUniformMatrix4fv("proj_mat", pj_mat, false);
		sphere_shader->setUniformMatrix4fv("view_mat", view_mat, false);
		sphere_shader->setUniformMatrix3fv("NormalMatrix", normal_mat, false);


        set_light_uniform(sphere_shader, depth_tex0 , light_index);
        set_light_uniform(sphere_shader, depth_tex1, light_index);
        set_light_uniform(sphere_shader, depth_tex2, light_index);

        glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
		glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
		geovector->Render();
		sphere_shader->disable();
		glPopMatrix();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		delete[] pj_mat;
		delete[] model_mat;
		delete[] view_mat;
        glActiveTexture(GL_TEXTURE0);
}
void shadow_pass( render2tex *depth_tex,std::vector<Geometry*> item , Geometry * light){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //gen texture or shadow map
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,depth_tex->fbo_id);  // fbo에다가 그려준다
        GLfloat pos_GL[4] = { double(light->orientation(3,0)), double(light->orientation(3,1)),double(light->orientation(3,2)), 1.0f};
        depth_tex->gen_shadow_map(pos_GL);
        for ( int i = 0 ; i < item.size(); i++ ){
                draw_shadow_geovector(geo_item[i]);
        }
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);  // fbo에다가 그려준다
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
void display() {

    shadow_pass( depth_tex0 , geo_item , light0);
    shadow_pass( depth_tex1 , geo_item , light1);
    shadow_pass( depth_tex2 , geo_item , light2);

//gen clear
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, 1260.0/760.0, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(current_map == 0)
            depth_tex0->draw_debug_texture();
    else if(current_map == 1)
            depth_tex1->draw_debug_texture();
    else if(current_map == 2)
            depth_tex2->draw_debug_texture();
    else
            current_map =0;

	camera.apply();

    draw_geovector(tray);
    draw_geovector(tablelamp);
    draw_geovector(geo);
    draw_light();

    pick.move_object(mouse_move);
	pick.get_mouse(Eigen::Vector2d(width,height),mouse_pose);
	pick.hit_update();
	drawGUI();
	glutSwapBuffers();
}
void init()
{
    glClearColor(0.0f, 0.0, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGLUT_Init();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION, Light_pos_0);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);
    depth_tex0->init_FBO("depth", 1000, 1000 , "point");
    depth_tex0->light = light0;
    depth_tex1->init_FBO("depth", 1000, 1000 , "point");
    depth_tex1->light = light1;
    depth_tex2->init_FBO("depth", 1000, 1000 , "directional");
    depth_tex2->light = light2;
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
                if(depth_tex0->light_switch == 1)
                        depth_tex0->light_switch = 0;
                else
                        depth_tex0->light_switch = 1;
                break;
        case '2':
                if(depth_tex1->light_switch == 1)
                        depth_tex1->light_switch = 0;
                else
                        depth_tex1->light_switch = 1;
                break;
        case '3':
                if(depth_tex2->light_switch == 1)
                        depth_tex2->light_switch = 0;
                else
                        depth_tex2->light_switch = 1;
                break;
        case 'n':
                current_map++;
                current_map = current_map % 3;
                break;
        }
        glutPostRedisplay();
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
	camera.resize(width, height);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// const GLubyte *e = glewGetErrorString(err);
		//printf("Error: %s\n", e);
	}
    depth_tex0 = new render2tex();
    depth_tex1 = new render2tex();
    depth_tex2 = new render2tex();

    shadow_shader = new glslShader();
	sphere_shader = new glslShader();
	const char* vs_shader_120 = "./shader/120version/120.vert" ;
	const char* fs_shader_120 = "./shader/120version/120.frag" ;
    const char* s_vs_shader_120 = "./shader/120version/120shadow.vert" ;
	const char* s_fs_shader_120 = "./shader/120version/120shadow.frag" ;

	sphere_shader->loadFromFile(vs_shader_120,fs_shader_120);
	shadow_shader->loadFromFile(s_vs_shader_120,s_fs_shader_120);

    light0 = new Geometry();
	light0->set_sphere(10,Light_pos_0[0],Light_pos_0[1],Light_pos_0[2]);

    light1 = new Geometry();
	light1->set_sphere(10,Light_pos_1[0],Light_pos_1[1],Light_pos_1[2]);

    light2 = new Geometry();
	light2->set_sphere(10,Light_pos_2[0],Light_pos_2[1],Light_pos_2[2]);

	geo = new Geometry();
	// geo->set_plain(100);
    geo->loadObj("./assets/plain.obj", 0, 0, 0);
    geo->scale_factor = 90;

    tablelamp = new Geometry();
    tablelamp->loadObj("./assets/tablelamp.obj", 20, 0, 20);

    tray = new Geometry();
    tray->loadObj("./assets/ashtray.obj",0,0,0);
    geo_item.push_back(geo);
    geo_item.push_back(tray);
    geo_item.push_back(tablelamp);

    pick.item.push_back(geo);
    pick.item.push_back(tablelamp);
    pick.item.push_back(light0);
    pick.item.push_back(light1);
    pick.item.push_back(light2);
    pick.item.push_back(tray);

    sphere_shader->enable();
	geo->setBuffer(sphere_shader->getProgId());
	tablelamp->setBuffer(sphere_shader->getProgId());
    tray->setBuffer(sphere_shader->getProgId());
    sphere_shader->disable();

    shadow_shader->enable();
    geo->setBuffer_shadow(shadow_shader->getProgId());
	tablelamp->setBuffer_shadow(shadow_shader->getProgId());
    tray->setBuffer_shadow(shadow_shader->getProgId());
    shadow_shader->disable();



    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
    init();
	glutMainLoop();

	return EXIT_SUCCESS;
}


void drawGUI()
{

	GLdouble *pj_mat = new GLdouble[16];
	GLdouble *model_mat = new GLdouble[16];
	GLint *identity = new GLint[4];
	glGetDoublev(GL_PROJECTION_MATRIX, pj_mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, model_mat);
	glGetIntegerv(GL_VIEWPORT,identity);
	Eigen::Matrix4d projection = GL2Eigen(pj_mat);
	Eigen::Matrix4d modelview = GL2Eigen(model_mat);
	if(pick.pick_button)
    {
		pick.modelview_mat = modelview;
		pick.projection_mat = projection;
		pick.pick_cal();
		pick.hit_check();
	}
    ImGui_ImplGLUT_NewFrame(width, height);
	ImGui::PushID("mouse pose");
	std::string hit1 = "no";std::string hit2 = "no";std::string hit3 = "no";
	std::string hit4 = "no";std::string hit5 = "no";
	if(tray->hit)hit1 = "HIT";
	if(tablelamp->hit)hit2 = "HIT";
	if(light0->hit)hit3 = "HIT";
    if(light1->hit)hit4 = "HIT";
    if(light2->hit)hit5 = "HIT";

	ImGui::Text("tray %s",hit1.c_str());
	ImGui::Text("tablelamp %s",hit2.c_str());
	ImGui::Text("light0 (white)%s",hit3.c_str());
	ImGui::Text("light1 (blue)%s",hit4.c_str());
	ImGui::Text("light2 (red)%s",hit5.c_str());

    if(ImGui::Button("On/Off Light 0")){
            depth_tex0->light_switch++;
            depth_tex0->light_switch = depth_tex0->light_switch %2;
            glutPostRedisplay();
    }
    if(depth_tex0->light_switch == 1){
            ImGui::SameLine(); ImGui::Text(" : ON");
    }
    else{
            ImGui::SameLine(); ImGui::Text(" : OFF");
    }
    if(ImGui::Button("On/Off Light 1")){
            depth_tex1->light_switch++;
            depth_tex1->light_switch = depth_tex1->light_switch %2;
            glutPostRedisplay();
    }
    if(depth_tex1->light_switch == 1){
            ImGui::SameLine(); ImGui::Text(" : ON");
    }
    else{
            ImGui::SameLine(); ImGui::Text(" : OFF");
    }
    if(ImGui::Button("On/Off Light 2")){
            depth_tex2->light_switch++;
            depth_tex2->light_switch = depth_tex2->light_switch %2;
            glutPostRedisplay();
    }
    if(depth_tex2->light_switch == 1){
            ImGui::SameLine(); ImGui::Text(" : ON");
    }
    else{
            ImGui::SameLine(); ImGui::Text(" : OFF");
    }
    ImGui::SliderInt("Debug Shadow Map", &current_map , 0, 2);
    ImGui::Text("\n\nSPHERE1 pos : %f,%f,%f",geo->orientation(3,0),geo->orientation(3,1),geo->orientation(3,2));
    ImGui::Text("SPHERE1 pos : %f,%f,%f",tablelamp->orientation(3,0),tablelamp->orientation(3,1),tablelamp->orientation(3,2));
    ImGui::Text("light0 pos : %f,%f,%f",light0->orientation(3,0),light0->orientation(3,1),light0->orientation(3,2));

    ImGui::Text("\n\nCONTROLL KEY : \nMouse right drag \nCtl + mouse right drag\nShift + mouse right drag");
    ImGui::Text("\nMouse Left click IS PICKING BUTTON");


    ImGui::PopID();
    ImGui::Render();
}


void inv_trans(float * input , float * output){
        Eigen::Matrix3f m;
        for(int i =0 ; i < 3 ; i++){
                m(0,i) = input[0+i*4];
                m(1,i) = input[1+i*4];
                m(2,i) = input[2+i*4];
        }
        Eigen::Matrix3f m2 = m.inverse();
        Eigen::Matrix3f m3 = m2.transpose();
        for(int i =0 ; i < 3 ; i++){
                output[0+3*i] = m3(0,i);
                output[1+3*i] = m3(1,i);
                output[2+3*i] = m3(2,i);
        }
}
void draw_light(){

        glColor3f(1,1,1);
        glPushMatrix();
        glTranslatef(light0->orientation(3,0), light0->orientation(3,1), light0->orientation(3,2));
        glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glColor3f(0,0,1);
        glPushMatrix();
        glTranslatef(light1->orientation(3,0), light1->orientation(3,1), light1->orientation(3,2));
        glutSolidSphere(1, 10, 10);
        glPopMatrix();

        glColor3f(1,0,0);
        glPushMatrix();
        glTranslatef(light2->orientation(3,0), light2->orientation(3,1), light2->orientation(3,2));
        glutSolidSphere(1, 10, 10);
        glPopMatrix();


}
