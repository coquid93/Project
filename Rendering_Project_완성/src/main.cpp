#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctime>
#include "../include/main_etc.h"
#include "../include/Geometry.h"
#include "../include/glslShader.h"
#include "../include/render2texture.h"

using namespace std;
#define light_num 13
std::vector<Geometry*> hw1;
Geometry* geo;
Geometry* tray;
Geometry* defer_quad;


glslShader* shadow_shader;
glslShader* sphere_shader;
glslShader* ssao_shader;
glslShader* defer_shader;

vector<render2tex *> light_map;
vector<Geometry * >  light_geo;
std::vector<Geometry*> geo_item;

render2tex *ssao;
static float ssao_radius = 10.f;
render2tex *defer;
float global_view_mat[16];
float global_proj_mat[16];
double time_diff;
static float ani_time = 0 ;
clock_t begin_clock=0 ;
clock_t end_clock=0 ;

static int current_map = 2;
static int deferred_render_switch = 0;
static int debugging_texture = 0;

string IntToString (int a)
{
        ostringstream temp;
        temp<<a;
        return temp.str();
}
void drawGUI();
void draw_light();
void keyboardCB(unsigned char keyPressed, int x, int y);
void set_light_uniform(glslShader * shader , render2tex * shadow_map , int &index);
void draw_object_all( std::vector<Geometry*> item){
//실제 적용해 보는것이 필요하다.
        int light_index = 0;

        sphere_shader->enable();
		sphere_shader->setUniformMatrix4fv("proj_mat", global_proj_mat, false);
		sphere_shader->setUniformMatrix4fv("view_mat", global_view_mat, false);
        for(int i = 0; i < light_num; i++){
                set_light_uniform(sphere_shader, light_map[i], light_index);
        }
// ssao part;
        sphere_shader->setUniform1f("uRadius" , ssao_radius);
        sphere_shader->setUniform1i("ssao_norm_tex", 15);
        glActiveTexture(GL_TEXTURE0 + 15);
        glBindTexture(GL_TEXTURE_2D, ssao->texture_norm_id);

        sphere_shader->setUniform1i("ssao_depth_tex", 16);
        glActiveTexture(GL_TEXTURE0 + 16);
        glBindTexture(GL_TEXTURE_2D, ssao->texture_rgb_id);

        sphere_shader->setUniform1i("defer_pos_tex", 17);
        glActiveTexture(GL_TEXTURE0 + 17);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_pos_id);

        glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
        for(int i=0 ; i < item.size(); i ++){
                sphere_shader->setUniformMatrix4fv("model_mat", item[i]->model_mat, false);
                item[i]->Render();
        }

		sphere_shader->disable();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
        glActiveTexture(GL_TEXTURE0);

}
void set_light_uniform(glslShader * shader , render2tex * shadow_map , int &index){
        string light_pos , light_view_mat , light_proj_mat , map_texture , turn ;
        string ind = IntToString(index);
        light_pos = "light[" + ind +"].position";
        light_view_mat = "light[" + ind +"].view_mat";
        light_proj_mat = "light[" + ind +"].proj_mat";
        map_texture = "light[" + ind +"].shadow_map";
        turn = "light[" + ind +"].turn";

        if( shadow_map->light_type == "point" )
                shader->setUniform4f(light_pos.c_str(), shadow_map->light->orientation(3,0), shadow_map->light->orientation(3,1), shadow_map->light->orientation(3,2),1.0 );
        else
                shader->setUniform4f(light_pos.c_str(), shadow_map->light->orientation(3,0), shadow_map->light->orientation(3,1), shadow_map->light->orientation(3,2),0.0 );

		shader->setUniformMatrix4fv(light_view_mat.c_str(), shadow_map->view_mat , false );
		shader->setUniformMatrix4fv(light_proj_mat.c_str(), shadow_map->proj_mat , false );
		shader->setUniform1i(map_texture.c_str(), index);
		shader->setUniform1i(turn.c_str(), shadow_map->light_switch);
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, shadow_map->texture_rgb_id);
        index++;
}
void draw_shadow_geovector(render2tex * shadow ,  Geometry* geovector){
		shadow_shader->enable();
		shadow_shader->setUniformMatrix4fv("model_mat", geovector->model_mat, false);
		shadow_shader->setUniformMatrix4fv("proj_mat", shadow->proj_mat, false);
		shadow_shader->setUniformMatrix4fv("view_mat", shadow->view_mat, false);
		geovector->Render_shadow();
		shadow_shader->disable();
}
void shadow_pass( render2tex *depth_tex,std::vector<Geometry*> item ){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //gen texture or shadow map
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,depth_tex->fbo_id);  // fbo에다가 그려준다
        GLfloat pos_GL[4] = { (depth_tex->light->orientation(3,0)), (depth_tex->light->orientation(3,1)),(depth_tex->light->orientation(3,2)), 1.0f};
        depth_tex->gen_shadow_map(pos_GL);
        for ( int i = 0 ; i < item.size(); i++ ){
                draw_shadow_geovector(depth_tex , item[i]);
        }
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);  // fbo에다가 그려준다
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
void deferred_rendering(){

        defer_shader->enable();
        int index = 0;
        for(int i =0; i< light_num ; i++){
                set_light_uniform(defer_shader, light_map[i], index);
        }
		defer_shader->setUniformMatrix4fv("proj_mat", global_proj_mat, false);
		defer_shader->setUniformMatrix4fv("view_mat", global_view_mat, false);
// ssao part;
        defer_shader->setUniform1f("uRadius" , ssao_radius);
        defer_shader->setUniform1i("ssao_norm_tex", 15);
        glActiveTexture(GL_TEXTURE0 + 15);
        glBindTexture(GL_TEXTURE_2D, ssao->texture_norm_id);

        defer_shader->setUniform1i("ssao_depth_tex", 16);
        glActiveTexture(GL_TEXTURE0 + 16);
        glBindTexture(GL_TEXTURE_2D, ssao->texture_rgb_id);

        defer_shader->setUniform1i("defer_pos_tex", 17);
        glActiveTexture(GL_TEXTURE0 + 17);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_pos_id);

        defer_shader->setUniform1i("defer_amb_tex", 18);
        glActiveTexture(GL_TEXTURE0 + 18);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_amb_id);

        defer_shader->setUniform1i("defer_dif_tex", 19);
        glActiveTexture(GL_TEXTURE0 + 19);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_diff_id);

        defer_shader->setUniform1i("defer_spe_tex", 20);
        glActiveTexture(GL_TEXTURE0 + 20);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_spec_id);

        defer_shader->setUniform1i("defer_ori_tex", 21);
        glActiveTexture(GL_TEXTURE0 + 21);
        glBindTexture(GL_TEXTURE_2D , ssao->texture_ori_id);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
        //render quad;
        defer_quad->Render();
        defer_shader->disable();
        glActiveTexture(GL_TEXTURE0);
        glDisable(GL_LIGHTING);
	    glDisable(GL_LIGHT0);
        glDisable(GL_DEPTH_TEST);
}
void display() {
    for( int i = 0 ; i< light_num ; i ++){
            shadow_pass( light_map[i] , geo_item);
    }
//gen clear
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,width,height);
    // camera.resize(camera.getScreenWidth(), camera.getScreenHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1260.0/760.0, 10, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.apply();

    glGetFloatv(GL_MODELVIEW_MATRIX, global_view_mat);
    glGetFloatv(GL_PROJECTION_MATRIX, global_proj_mat);
    //gen texture or shadow map
    ssao->gen_ssao_map(ssao_shader, geo_item);   //shadow_shader 를 이용해서 그림. z = depth  가 된다.

    static int frame = 0 , time___, timebase = 0;
    time___ = glutGet(GLUT_ELAPSED_TIME);
    frame ++;
    if (time___ - timebase > 1000) {
            time_diff = float(frame)*1000.0/float(time___-timebase);
            timebase = time___;
            frame = 0;
	}
    if(deferred_render_switch == 0)
    {
            draw_object_all(geo_item );
    }
    else
    {
            deferred_rendering();
    }

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
    ssao->init_FBO("ssao", width, height , "ssao");
}

void animation(int i){
	if(ani_time > 20)
		ani_time = 0;
	ani_time+= i;
	glutPostRedisplay();
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
	camera.resize(width, height);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// const GLubyte *e = glewGetErrorString(err);
		//printf("Error: %s\n", e);
	}


    render2tex * tex_array[light_num];
    for(int i=0; i<light_num; i++){
            tex_array[i] = new render2tex();
            tex_array[i]->init_FBO("depth", 1000, 1000, "point");
            light_map.push_back( tex_array[i] );
    }
    Geometry * light_geo_array[light_num];
    for(int i=0; i<light_num; i++){
            light_geo_array[i] = new Geometry();
            light_geo_array[i]->set_light( 10,Light_pos_0[1],Light_pos_0[1],Light_pos_0[1]);
            light_geo_array[i]->trans_geo(Eigen::Vector3d(-i*10 , 0 , -i*10));
            light_map[i]->light = light_geo_array[i];
            light_geo.push_back( light_geo_array[i] );
            pick.item.push_back(light_geo_array[i]);
    }
    light_geo_array[light_num-1]->trans_geo(Eigen::Vector3d(0 , 0 , 0.1));
    ssao = new render2tex();
    defer = new render2tex();

    shadow_shader = new glslShader();
	sphere_shader = new glslShader();
	ssao_shader = new glslShader();
    defer_shader = new glslShader();

    const char* vs_shader_120 = "../shader/120version/120.vert" ;
	const char* fs_shader_120 = "../shader/120version/120.frag" ;
    const char* s_vs_shader_120 = "../shader/120version/120shadow.vert" ;
	const char* s_fs_shader_120 = "../shader/120version/120shadow.frag" ;
    const char* ssao_vs_shader_120 = "../shader/120version/120r2t.vert" ;
	const char* ssao_fs_shader_120 = "../shader/120version/120r2t.frag" ;
    const char* defer_vs_120 = "../shader/120version/defer.vert" ;
	const char* defer_fs_120 = "../shader/120version/defer.frag" ;
	sphere_shader->loadFromFile(vs_shader_120,fs_shader_120);
	shadow_shader->loadFromFile(s_vs_shader_120,s_fs_shader_120);
	ssao_shader->loadFromFile(ssao_vs_shader_120,ssao_fs_shader_120);
    defer_shader->loadFromFile(defer_vs_120, defer_fs_120);



    geo = new Geometry();
    geo->scale_factor = 1.0f;
    geo->loadObj("../assets/plain.obj", 0, 0, 0);
    geo->set_amb_dif_spec(glassAmbient, glassDiffuse, glassSpecular);

    tray = new Geometry();
    tray->scale_factor = 5.0f;
    tray->loadObj("../assets/bunny.obj",0,1,0);

    Geometry *bunny1 = new Geometry();
    bunny1->scale_factor = 3.0f;
    bunny1->copy_geometry(tray);
    bunny1->trans_geo(Eigen::Vector3d(0,0,30));
    for(int i = 0 ; i < 8 ; i++){
            for( int j = 0 ; j < 8 ; j ++){
                    Geometry *bunny = new Geometry();
                    bunny->scale_factor = 3.0f;
                    bunny->copy_geometry(tray);
                    bunny->trans_geo(Eigen::Vector3d(-60 + 20*i , 0 , -60 + 20 * j));
                    geo_item.push_back(bunny);
                    pick.item.push_back(bunny);
            }
    }
    geo_item.push_back(geo);
    pick.item.push_back(geo);

    sphere_shader->enable();
    for(int i=0 ; i < geo_item.size(); i++){
            geo_item[i]->setBuffer(sphere_shader->getProgId());
    }
    sphere_shader->disable();

    shadow_shader->enable();
    for(int i=0 ; i < geo_item.size(); i++){
            geo_item[i]->setBuffer_shadow(shadow_shader->getProgId());
    }
    shadow_shader->disable();

    ssao_shader->enable();
    for(int i=0 ; i < geo_item.size(); i++){
            geo_item[i]->setBuffer_shadow(ssao_shader->getProgId());
    }
    ssao_shader->disable();

    defer_shader->enable();
    defer_quad = new Geometry();
    defer_quad->set_plain(1.0f);
    defer_quad->setBuffer(defer_shader->getProgId());
    defer_shader->disable();


    float * vec3;
    vec3 = new float[90];
    sphere_shader->enable();
    for(int i=0 ; i< 90 ; i++){
            string ind = IntToString(i);
            vec3[i] = rand();
            string name_space = "kernel_r["+ind+"]";
            sphere_shader->setUniform3f(name_space.c_str(),vec3[i], vec3[i+1], vec3[i+2]);
    }
    sphere_shader->disable();

    defer_shader->enable();
    for(int i=0 ; i< 90 ; i++){
            string ind = IntToString(i);
            string name_space = "kernel_r["+ind+"]";
            sphere_shader->setUniform3f(name_space.c_str(),vec3[i], vec3[i+1], vec3[i+2]);
    }
    defer_shader->disable();
    delete [] vec3;

	glutTimerFunc(10,animation,1);
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

    for ( int i = 0 ; i < light_num; i++){
            std::string l = "Light";
            l += IntToString(i);
            if(ImGui::Button(l.c_str())){
                    light_map[i]->light_switch++;
                    light_map[i]->light_switch %= 2;
                    glutPostRedisplay();
            }
            ImGui::SameLine();
            if(light_map[i]->light_switch == 1){ImGui::Text("ON");}
            else{ImGui::Text("OFF");}
    }
    if(ImGui::Button("Deferred Rendering")){
            deferred_render_switch ++;
            deferred_render_switch %= 2;
            glutPostRedisplay();
    }
    ImGui::SameLine();
    if(deferred_render_switch == 1){ImGui::Text("ON");}
    else{ImGui::Text("OFF");}
	ImGui::SameLine();
	ImGui::Text("      FPS = %f",time_diff);
    if(ImGui::Button("debug_texture")){
            debugging_texture ++;
            debugging_texture %=2;
            glutPostRedisplay();
    }
    ImGui::SameLine();
    if(debugging_texture == 1){ImGui::Text("ON");}
    else{ImGui::Text("OFF");}
    ImGui::SliderInt("Debug Shadow Map", &current_map , 0, light_num -1 + 6 );
    if(debugging_texture == 1){
            if(current_map < light_num )
                    light_map[current_map]->draw_debug_texture("depth");
            else if(current_map == light_num)
                    ssao->draw_debug_texture("position");
            else if ( current_map == light_num+1)
                    ssao->draw_debug_texture("depth");
            else if ( current_map == light_num + 2)
                    ssao->draw_debug_texture("normal");
            else if ( current_map == light_num + 3)
                    ssao->draw_debug_texture("ambient");
            else if(current_map == light_num + 4)
                    ssao->draw_debug_texture("diffuse");
            else if (current_map == light_num +5)
                    ssao->draw_debug_texture("specular");
    }
    ImGui::SliderFloat("SSAO Radius", &ssao_radius, 0.1f, 15.0f);
	ImGui::Text("time = %f",ani_time);
    ImGui::Text("\n\nCONTROLL KEY : \nMouse right drag \nCtl + mouse right drag\nShift + mouse right drag");
    ImGui::Text("\nMouse Left click IS PICKING BUTTON");


    ImGui::PopID();
    ImGui::Render();
}

void draw_light(){
        for( int i = 0 ; i < light_num ; i++){
                glColor3f( sin(double(i)) , cos(double(i)) , tan(double(i) ));
                glPushMatrix();
                glTranslatef(light_geo[i]->orientation(3,0), light_geo[i]->orientation(3,1), light_geo[i]->orientation(3,2));
                glutSolidSphere(1, 10, 10);
                glPopMatrix();
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
        case 'n':
                current_map++;
                current_map = current_map % 3;
                break;
        }
        glutPostRedisplay();
}

