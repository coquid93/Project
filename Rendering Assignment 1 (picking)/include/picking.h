/*
 * picking.h
 *
 *  Created on: Mar 27, 2017
 *      Author: luislee
 */

#include "../external/GL/freeglut.h"
#include "../external/Eigen/Eigen"
#include "Geometry.h"
#include <iostream>
#include <vector>

Eigen::Matrix4d GL2Eigen(GLdouble* mat);
GLdouble* Eigen2GL(Eigen::Matrix4d &mat);


class Picking {
public:
	void get_mouse(Eigen::Vector2d& viewport,Eigen::Vector3d mouse_pos);
	void hit_update();
	void pick_cal();

	void hit_check();
	void move_object(Eigen::Vector3d mouse_move);
public:
	Eigen::Vector3d normalized_pos;
	Eigen::Vector3d world_pos;
	Eigen::Matrix4d projection_mat;
	Eigen::Matrix4d modelview_mat;
	Eigen::Vector3d near_;
	Eigen::Vector3d far_;
	bool pick_button;
	bool moving;
	std::vector<Geometry*> item;

	Eigen::Vector4d move_direction;
};

