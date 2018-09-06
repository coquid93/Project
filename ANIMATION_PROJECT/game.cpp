/*
 * game.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: luislee
 */




#include "game.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Game::draw_object(){
	glPushMatrix();
	glColor3fv(obj_color);
	glScalef(0.1,0.1,0.1);
	glTranslatef(obj_pos[0],10,obj_pos[2]);
	glutSolidCube(20);
	glPopMatrix();
}
void Game::move_object(){
	srand(u_int( time(NULL)));
	float x = rand() % 1000;
	srand(u_int( 3*time(NULL)));
	float z = rand() % 1000;
	cout<<x<< " , "<<z<<endl;
	obj_pos[0] = x;
	obj_pos[2] = z;
	obj_pos[1] = 0;
}
bool Game::collision(Body_structure B){
	time_t tmp = clock()-start_time;
	lest_time = CLOCKS_PER_SEC*tt-tmp;
	bool hit =false;
	glm::vec3 temp = B.root_pos_[B.frame_step]; temp[1] = 0;
	glm::vec3 dist = temp-obj_pos;
	float distance = dist[0]*dist[0]+dist[2]*dist[2];
	distance = glm::sqrt(distance);
	if(distance < 100) {
		hit =true;cout<<"hit"<<endl;
		score++;
		cout<<"score:"<<score<<endl;
	}
	return hit;

}
