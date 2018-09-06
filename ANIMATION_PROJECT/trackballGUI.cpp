#include <iostream>
#include <GL/freeglut.h>
#include <time.h>
#include "Luis/Body_structure.h"
#include "Luis/camera.h"
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "MotionCalc.h"
#include "game.h"
#define BUFFER_LENGTH 64
static Camera camera;
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
static unsigned int width = 1280;
static unsigned int height = 720;
static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static int lastX = 0, lastY = 0, lastZoom = 0;

static bool first_call = true;

static bool fullScreen = false;

Body_structure Body;
Body_structure Body_left;
Body_structure Body_right;
static Body_structure b[10];
std::vector<Body_structure> Body_set;


Read_BVH *R;
Read_BVH *nextR;

static std::vector<Read_BVH> walk_left;
static std::vector<Read_BVH> walk_right;
static std::vector<Read_BVH> walk_forward;
static std::vector<Read_BVH> event_bvh;
static std::vector<Body_structure> body_set_forward;
static std::vector<Body_structure> body_set_left;
static std::vector<Body_structure> body_set_right;
static std::vector<Body_structure> body_event_motion;

static Body_structure *forward_body;
static Body_structure *left_body;
static Body_structure *right_body;
static int forward_body_ind=0;
static int left_body_ind=0;
static int right_body_ind=0;
static bool design = false;

static int anim_state =0;
static int anim_state_before =0;
static bool click_merge = false;
static bool finish_loop = true;

static float  play_speed = 1.0f;
static int stop =0;
glm::vec3 offset_pos;


static bool show;
Node* target;
glm::vec3 pos_d;
glm::vec3 t_pos;
using namespace std;

Body_structure Test;
Body_structure Test1;
Body_structure Test2;
std::vector<std::vector<Body_structure > > Set_body;
static int direction_ind =0;
static int stride_ind =5;
static Game game;
static int game_state =0;
Blending cor;
void make_loop_animation();
void drawGUI()
{
    ImGui_ImplGLUT_NewFrame(width, height);
//    ImGui::PushID("Body_ind");
//    const char* Body_type[] = { "slow", "normal", "fast" , "left45","left90","left135","left180",
//    		"right45","right90","right135","right180"};
//    ImGui::ListBox("", &Body_ind, Body_type, IM_ARRAYSIZE(Body_type),6 );
//    ImGui::PopID();
//    ImGui::PushID("111");
//	ImGui::SliderInt("frame1", &Body_set[Body_ind].frame_step, 0, Body.total_frame_num-1);
//	ImGui::SameLine(0,1);
//	if(ImGui::Button("-")) Body_set[Body_ind].frame_step --;
//	ImGui::SameLine(0,1);
//	if(ImGui::Button("+")) Body_set[Body_ind].frame_step++;
//	ImGui::PopID();
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
//    if(ImGui::TreeNode("PA4")){
//		if(ImGui::Button("PLAY")){ stop = 2;finish_loop = false;}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("STOP")) stop = 0;
//
//    	if(ImGui::Button("PLAY DESIGNED ANIMATION")) {stop = 2;design = true;}
//    	ImGui::PushID("merge list");
//    	if(ImGui::Button("slow")){
//    		if(finish_loop){ anim_state =0;click_merge = true;finish_loop =false;}
//    	}
//    	ImGui::SameLine(0,1);
//    	if(ImGui::Button("normal")){
//			if(finish_loop){ anim_state =1;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("fast")){
//			if(finish_loop){ anim_state =2;click_merge = true;finish_loop =false;}
//		}
//		ImGui::PushID("L");
//		ImGui::Text("left");
//    	ImGui::SameLine(0,1);
//    	if(ImGui::Button("45")) {
//			if(finish_loop){anim_state =3;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("90")) {
//			if(finish_loop){anim_state =4;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("135")) {
//			if(finish_loop){anim_state =5;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("180")) {
//			if(finish_loop){anim_state =6;click_merge = true;finish_loop =false;}
//		}
//		ImGui::PopID();
//		ImGui::PushID("R");
//		ImGui::Text("Right");
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("45")) {
//			if(finish_loop){anim_state =7;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("90")) {
//			if(finish_loop){anim_state =8;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("135")) {
//			if(finish_loop){anim_state =9;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("180")) {
//			if(finish_loop){anim_state =10;click_merge = true;finish_loop =false;}
//		}
//		ImGui::PopID();
//		if(ImGui::Button("jump")) {
//			if(finish_loop){anim_state =11;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("high")) {
//			if(finish_loop){anim_state =12;click_merge = true;finish_loop =false;}
//		}
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("forward_jump")) {
//			if(finish_loop){anim_state =13;click_merge = true;finish_loop =false;}
//		}
//		ImGui::PopID();
//		ImGui::PushID("4");
//		ImGui::Text("Play Speed , %.3f,", play_speed);
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("-")) play_speed -= 0.05;
//		ImGui::SameLine(0,1);
//		if(ImGui::Button("+")) play_speed += 0.1;
//		ImGui::PopID();
//		ImGui::TreePop();
//    }
    if(ImGui::TreeNode("PA5")){
    	ImGui::Text("Not yet");
    	ImGui::Checkbox("Show Character",&show);
    	if(show) {}else {}
    	ImGui::SliderInt("Test1", &Test1.frame_step, Test1.edit_point[0], Test1.edit_point[1]);
    	ImGui::SliderInt("Test2", &Test2.frame_step, Test2.edit_point[0], Test2.edit_point[1]);
    	ImGui::SliderInt("Test", &Test.frame_step, Test.edit_point[0], Test.edit_point[1]);
    	ImGui::TreePop();
    }
    ImGui::PushID("DIRECTION");
    int x1=0;
    if(ImGui::Button("START GAME")){game_state = 1; stop =2; game.start_time = clock();}
    ImGui::SameLine(0,2);
    if(ImGui::Button("STOP GAME")){ game_state = 0; stop =0; game.reset_time();game.score =0;}

	if(ImGui::Button("PAUSE")){ game_state = 2; stop =0;}
	ImGui::SameLine(0,2);
	if(ImGui::Button("RESTART")){ game_state = 1; stop =2; game.start_time = clock();}

    ImGui::SliderInt("DERECTION",&direction_ind,-5,5);
    ImGui::SliderInt("STRIDE",&stride_ind,3,5);
    ImGui::Text("SCORE : %i AND TIME : %i",game.score,game.lest_time/CLOCKS_PER_SEC);
    ImGui::PopID();
    ImGui::Render();
}
void drawBody_merge(Body_structure &B){
	if(first_call){
		B.start_time = clock();
		first_call = false;
	}
	B.current_time = clock();
	switch(stop){
	case 2:	//재생
		if((B.current_time-B.start_time)*play_speed > CLOCKS_PER_SEC*B.total_frame_time){
			B.start_time = clock();
			B.current_time = clock();
			if(B.frame_step < B.edit_point[1]){//멈춤
				B.frame_step++;
			}
		}
		break;
	default:
		break;
	}
	if(B.frame_step == B.edit_point[1]) finish_loop= true;
	B.cliping_frame(B.edit_point[0],B.edit_point[1]);
	B.update_by_bvh(int(B.frame_step),&B.root);
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	B.root.draw_node();
	glPopMatrix();
}
bool play_body_clip(Body_structure &B){
	bool end_clip =false;
	if(B.frame_step < B.edit_point[0]) {
		B.frame_step =B.edit_point[0];
	}
	if(B.frame_step == B.edit_point[1]) {
		end_clip = true;
	}

	drawBody_merge(B);
	return end_clip;
}
bool play_body_clip_with_blend(Body_structure &B,Body_structure &B2){
	bool end_clip =false;
	if(B.frame_step < B.edit_point[0]) {
		B.frame_step =B.edit_point[0];
	}
	if(B.frame_step == B.edit_point[1]) {
		end_clip = true;
	}
	Blending BB;
	cout<<"here"<<endl;
	cor.Blend_at_moment(B,B2,0.5);
	cout<<"here"<<endl;
	drawBody_merge(B);
	return end_clip;
}
Body_structure* find_body(int x){
	Body_structure* B;
	switch(x){
	case 0:
		B = &body_set_forward[0];
		break;
	case 1:
		B = &body_set_forward[1];
		break;
	case 2:
		B = &body_set_forward[2];
		break;
	case 3:
		B =& body_set_left[0];
		break;
	case 4:
		B = &body_set_left[1];
		break;
	case 5:
		B = &body_set_left[2];
		break;
	case 6:
		B = &body_set_left[3];
		break;
	case 7:
		B = &body_set_right[0];
		break;
	case 8:
		B = &body_set_right[1];
		break;
	case 9:
		B = &body_set_right[2];
		break;
	case 10:
		B = &body_set_right[3];
		break;
	case 11:
		B = &body_event_motion[0];
		break;
	case 12:
		B = &body_event_motion[1];
		break;
	case 13:
		B = &body_event_motion[2];
		break;
	default:
		break;
		}
	return B;
}
void game_play(){
	 static Body_structure current_body_ = Set_body[5][5];
	 static Body_structure next_body_;
	glm::vec2 key; key[0] = direction_ind+5; key[1] = stride_ind+5;
	bool end__ = play_body_clip(current_body_);
//	if(stride_ind == 4 && first_){
//		Blending BBB;
//		next_body_ = Body_set[Body_set.size()-1];
//		next_body_.break_chain(next_body_);
//		Body_structure Bod;
//		Bod = BBB.Blend_at_moment(current_body_,next_body_,0.5);
//		MotionCalc m;
//		cout<<"Body exist"<<endl;
//		m.Align_motion_at(current_body_,current_body_.frame_step,Bod,Bod.frame_step);
//		current_body_ = Bod;
//		current_body_.break_chain(current_body_);
//		first_ = false;
//	}
	if(end__){
		cout<<"key = "<<key[0]<<" , "<<key[1]<<endl;
		next_body_ = Set_body[stride_ind][direction_ind+5];
		next_body_.break_chain(next_body_);
		MotionCalc m;
		cout<<"Body exist"<<endl;
		m.change_motion(current_body_,current_body_.edit_point[1],next_body_,next_body_.edit_point[0]);
		current_body_ = next_body_;
		current_body_.break_chain(current_body_);
	}
	game.draw_object();
	if(game_state !=2){
		if(game.collision(current_body_)){
			game.move_object();
		}
	}
}
void Set_body_draw(){//두 애니메이션을 합치는것만 하면 됨.

	for(int i=0;i<11;i++){
		play_body_clip(Set_body[5][i]);
	}
	for(int i=0;i<11;i++){
		play_body_clip(Set_body[4][i]);
	}
	for(int i=0;i<11;i++){
		play_body_clip(Set_body[3][i]);
	}
}
void draw_designed(){
	static int ind = 0;
	bool next = play_body_clip(b[ind]);
	if(next){
		ind++;
	}
	if(ind == 10 ){
		stop =0;
		design = false;
		ind = 0;
	}
	if(stop == 0){
		ind = 0; design = false;
	}
}
void draw_floor(){
	glPushMatrix();
	glScalef(50,50,50);
	glTranslatef(-5,0,-5);
	glColor3f(.3,.3,.3);
	glBegin(GL_QUADS);
	glVertex3f( 0,-0.001, 0);
	glVertex3f( 0,-0.001,10);
	glVertex3f(10,-0.001,10);
	glVertex3f(10,-0.001, 0);
	glEnd();

	glBegin(GL_LINES);
	for(int i=0;i<=10;i++) {
	    if (i==0) { glColor3f(.6,.3,.3); } else { glColor3f(.25,.25,.25); };
	    glVertex3f(i,0,0);
	    glVertex3f(i,0,10);
	    if (i==0) { glColor3f(.3,.3,.6); } else { glColor3f(.25,.25,.25); };
	    glVertex3f(0,0,i);
	    glVertex3f(10,0,i);
	};
	glEnd();
	glPopMatrix();
}
void display() {
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    glTranslatef(-offset_pos[0]/10,-offset_pos[1]/10,-offset_pos[2]/10);
    draw_floor();
    //for check
    if(design){
	draw_designed();
    }
    //loop_animation();
    if(show){
    	stop = 2;
    	play_speed = 0.3;
    	//play_body(Body_set[0],Body_set[0].edit_point[0],Body_set[0].edit_point[1]);
//    	play_body_clip(Test1);
//    	play_body_clip(Test2);
//    	play_body_clip(Test);
    	Set_body_draw();
    }
    //play_body_clip_with_blend(Body_set[0],Body_set[6]);
    if(game_state !=0){
    	play_speed = 1;
    	game_play();
    }
    drawGUI();
    glutSwapBuffers();
    glutPostRedisplay();

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
    case 'u':
		camera.sky_view();
    	break;
    case 'p':
    	stop = 1;
    	break;
    case 'q':
        exit(0);
        break;
    case 'w':
		if(stride_ind <5) stride_ind++;
		break;
    case 's':
		if(stride_ind >3) stride_ind--;
		break;
    case 'd':
		if(direction_ind <5) direction_ind++;
		break;
    case 'a':
		if(direction_ind > -5) direction_ind--;
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
        } else if (button==GLUT_RIGHT_BUTTON) {
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
void mouseMoveCallback(int x, int y)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)x, (float)y);

    glutPostRedisplay();
}
void idle() {

}
void init()
{
    glClearColor(0.447f, 0.565f, 0.604f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplGLUT_Init();
}
void Read_working_data(){
	Read_BVH left45;			left45.read_data("./Luis/BVH_folder/mrl/walk_normal_left_45.bvh");					walk_left.push_back(left45);
	Read_BVH left90;			left90.read_data("./Luis/BVH_folder/mrl/walk_normal_left_90.bvh");					walk_left.push_back(left90);
	Read_BVH left135;			left135.read_data("./Luis/BVH_folder/mrl/walk_normal_left_135.bvh");				walk_left.push_back(left135);
	Read_BVH left180;			left180.read_data("./Luis/BVH_folder/mrl/walk_normal_left_180.bvh");				walk_left.push_back(left180);

	Read_BVH Right45;			Right45.read_data("./Luis/BVH_folder/mrl/walk_normal_right_45.bvh");				walk_right.push_back(Right45);
	Read_BVH Right90;			Right90.read_data("./Luis/BVH_folder/mrl/walk_normal_right_90.bvh");				walk_right.push_back(Right90);
	Read_BVH Right135;			Right135.read_data("./Luis/BVH_folder/mrl/walk_normal_right_135.bvh");				walk_right.push_back(Right135);
	Read_BVH Right180;			Right180.read_data("./Luis/BVH_folder/mrl/walk_normal_right_180.bvh");				walk_right.push_back(Right180);

	Read_BVH forward_slow;		forward_slow.read_data("./Luis/BVH_folder/mrl/walk_slow_stright.bvh");				walk_forward.push_back(forward_slow);
	Read_BVH forward_normal;	forward_normal.read_data("./Luis/BVH_folder/mrl/walk_normal_stright.bvh");			walk_forward.push_back(forward_normal);
	Read_BVH forward_fast;		forward_fast.read_data("./Luis/BVH_folder/mrl/walk_fast_stright.bvh");				walk_forward.push_back(forward_fast);

	Body_structure Body_slow ;			forward_slow.Create_body(Body_slow);			Body_slow.edit_point = glm::vec2(120,224);				body_set_forward.push_back(Body_slow);			Body_set.push_back(Body_slow);
	Body_structure Body_normal ;		forward_normal.Create_body(Body_normal);		Body_normal.edit_point = glm::vec2(103,143);			body_set_forward.push_back(Body_normal);		Body_set.push_back(Body_normal);
	Body_structure Body_fast ;			forward_fast.Create_body(Body_fast);			Body_fast.edit_point = glm::vec2(108,170);				body_set_forward.push_back(Body_fast);			Body_set.push_back(Body_fast);
	Body_structure Body_left45 ;		left45.Create_body(Body_left45);				Body_left45.edit_point = glm::vec2(138,220);			body_set_left.push_back(Body_left45);			Body_set.push_back(Body_left45);
	Body_structure Body_left90 ;		left90.Create_body(Body_left90);				Body_left90.edit_point = glm::vec2(115,162);			body_set_left.push_back(Body_left90);			Body_set.push_back(Body_left90);
	Body_structure Body_left135 ;		left135.Create_body(Body_left135);				Body_left135.edit_point = glm::vec2(128,210);			body_set_left.push_back(Body_left135);			Body_set.push_back(Body_left135);
	Body_structure Body_left180 ;		left180.Create_body(Body_left180);				Body_left180.edit_point = glm::vec2(117,223);			body_set_left.push_back(Body_left180);			Body_set.push_back(Body_left180);
	Body_structure Body_right45 ;		Right45.Create_body(Body_right45);				Body_right45.edit_point = glm::vec2(104,177);			body_set_right.push_back(Body_right45);			Body_set.push_back(Body_right45);
	Body_structure Body_right90 ;		Right90.Create_body(Body_right90);				Body_right90.edit_point = glm::vec2(112,152);			body_set_right.push_back(Body_right90);			Body_set.push_back(Body_right90);
	Body_structure Body_right135 ;		Right135.Create_body(Body_right135);			Body_right135.edit_point = glm::vec2(78,197);			body_set_right.push_back(Body_right135);		Body_set.push_back(Body_right135);
	Body_structure Body_right180 ;		Right180.Create_body(Body_right180);			Body_right180.edit_point = glm::vec2(56,173);			body_set_right.push_back(Body_right180);		Body_set.push_back(Body_right180);

	Read_BVH jump;						jump.read_data("./Luis/BVH_folder/mrl/jump.bvh");
	Read_BVH high_jump;					high_jump.read_data("./Luis/BVH_folder/mrl/high jump.bvh");
	Read_BVH forward_jump;				forward_jump.read_data("./Luis/BVH_folder/mrl/forward jump.bvh");
	Read_BVH T_pose;					T_pose.read_data("./Luis/BVH_folder/mrl/tpose2.bvh");

	Body_structure Body_jump;			jump.Create_body(Body_jump);					Body_jump.edit_point = glm::vec2(1,79);					body_event_motion.push_back(Body_jump);
	Body_structure Body_high_jump;		high_jump.Create_body(Body_high_jump);			Body_high_jump.edit_point = glm::vec2(1,101);			body_event_motion.push_back(Body_high_jump);
	Body_structure Body_forward_jump;	forward_jump.Create_body(Body_forward_jump);	Body_forward_jump.edit_point = glm::vec2(1,72);			body_event_motion.push_back(Body_forward_jump);
	Body_structure Body_T_pose;			T_pose.Create_body(Body_T_pose);				Body_T_pose.edit_point = glm::vec2(617,671);
	event_bvh.push_back(jump);event_bvh.push_back(high_jump);event_bvh.push_back(forward_jump);
	Body_set.push_back(Body_jump);		Body_set.push_back(Body_high_jump);		Body_set.push_back(Body_forward_jump);
	Body_set.push_back(Body_T_pose);
}
void make_loop_animation(){

	b[0] = Body_set[1];
	b[1] = Body_set[2];
	b[2] = Body_set[6];
	b[3] = Body_set[4];
	b[4] = Body_set[11];
	b[5] = Body_set[0];
	b[6] = Body_set[10];
	b[7] = Body_set[7];
	b[8] = Body_set[3];
	b[9] = Body_set[5];

	for(int i=0;i<9;i++){
		MotionCalc m;
		cout<<b[i].edit_point[0]<<" "<<b[i].edit_point[1]<<endl;
		m.change_motion(b[i],b[i].edit_point[1],b[i+1],b[i+1].edit_point[0]);
		b[i+1].frame_step = b[i+1].edit_point[0];
	}

}

int main(int argc, char** argv) {
	Set_body.resize(6);
	for (int var = 0; var < 6; var++) {
		Set_body[var].resize(11);
	}
	Read_working_data();
	//make_loop_animation();
	R = &walk_forward[0];
	R->Create_body(Body);
	R = &walk_left[0];
	R->Create_body(Body_left);
	R = &walk_right[0];
	R->Create_body(Body_right);
	offset_pos = Body.root_pos_[0];
	Body.update_by_bvh(0,&Body.root);
	Body_left.update_by_bvh(0,&Body_left.root);
	Body.frame_step = 120;
	Body_left.frame_step= 0;
	Test1 = Body_set[1];
	Test2 = Body_set[4];
	Test1.break_chain(Test1);
	Test2.break_chain(Test2);
	Blending blend;
	Test1.frame_step = 120;
	Test  = blend.Blend_at_moment(Test1,Test2,0.5);
	for(int i=0;i<5;i++){
		Body_structure B_,B1,B2;
		B1 = Body_set[1]; B1.break_chain(B1);
		B2 = Body_set[4]; B2.break_chain(B2);
		Blending BL_;
		float w = 0.2*float(i);
		B_ = BL_.Blend_Motion(B1,B2,w);

		Set_body[5][i] = B_;
	}
	for(int i=6;i<11;i++){
		Body_structure B_,B1,B2;Blending BL_;
		B1 = Body_set[1]; B1.break_chain(B1);
		B2 = Body_set[8]; B2.break_chain(B2);
		float w = 1-0.2*float(i-5);
		B_ = BL_.Blend_Motion(B1,B2,w);
		Set_body[5][i] = B_;
	}
	Set_body[5][10] = Body_set[8];
	Set_body[5][0] = Body_set[4];
	Set_body[5][5] = Body_set[1];
	Blending BBBB;

	for(int i=3;i<5;i++){
		for(int j=0;j<11;j++){
			cout<< i <<" , "<< j<<endl;
			Body_structure B_,B1,B2;Blending BL_;
			B1 = Set_body[5][j]; B1.break_chain(B1);
			B2 = Body_set[Body_set.size()-1]; B2.break_chain(B2);
			float w = float(i-2)*0.35;
			B_ = BL_.Blend_Motion(B1,B2,w);
			Set_body[i][j] = B_;

		}
	}
	BBBB.align_traj(Body_set[1],Set_body[5][10]);
		BBBB.align_traj(Body_set[1],Set_body[5][0]);
	//Set_body[5][6] = Test;
	//Set_body[5][4] = Body_set[4];
	//blend.matching(Body_set[0],Body_set[2]);
	cor.Blend_Motion(Body_set[0],Body_set[6],1);
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

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);
    glutPassiveMotionFunc(mouseMoveCallback);
    init();
    glutMainLoop();

    ImGui_ImplGLUT_Shutdown();
    return 0;
}
