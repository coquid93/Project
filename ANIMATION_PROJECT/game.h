/*
 * game.h
 *
 *  Created on: Dec 13, 2016
 *      Author: luislee
 */

#ifndef ANIMATION_PRO_GAME_H_
#define ANIMATION_PRO_GAME_H_

#include <GL/freeglut.h>
#include "Luis/Eigen/Dense"
#include "Luis/glm/common.hpp"
#include "Luis/glm/ext.hpp"
#include "Luis/Body_structure.h"
#include <vector>
static int tt = 100;
class Game{
public:
	//somethings
	Game(){obj_pos = glm::vec3(100,0,0); obj_color[0] = 1;obj_color[1] = 0;obj_color[2] = 0;score = 0;start_time = CLOCKS_PER_SEC*100;lest_time = CLOCKS_PER_SEC*tt;}
	void draw_object();
	void move_object();
	void reset_time(){
		lest_time = CLOCKS_PER_SEC*tt;
	}
	bool collision(Body_structure B);
	void score_func();
	GLfloat body_color[3];
	GLfloat obj_color[3];
	time_t start_time;
	time_t lest_time;
	glm::vec3 obj_pos;
	int score;
};

#endif /* ANIMATION_PRO_GAME_H_ */
