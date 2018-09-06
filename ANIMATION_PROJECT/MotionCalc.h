/*
 * MotionCalc.h
 *
 *  Created on: Nov 18, 2016
 *      Author: luislee
 */

#ifndef ANIMATION_PRO_MOTIONCALC_H_
#define ANIMATION_PRO_MOTIONCALC_H_
#include "Luis/Eigen/Dense"
#include "Luis/glm/common.hpp"
#include "Luis/Body_structure.h"
#include <vector>
using namespace Eigen;
const static float radian = 57.2957;
class motion_vec{
public:
	motion_vec(){}
	motion_vec(glm::vec3 root,glm::vec3* motion,int size){
		root_position(0) = root[0];
		root_position(1) = root[1];
		root_position(2) = root[2];
		root_ori = euler2quat(motion[0]);//root_ori
		for(int i=1;i<size;i++){
			non_root.push_back(euler2quat(motion[i]));
		}
	}
	Quaternionf euler2quat(glm::vec3 ZXY){
		Quaternionf result;
		Matrix3f rot;
		rot = AngleAxisf(ZXY[0]/radian, Vector3f::UnitZ())
		  * AngleAxisf(ZXY[1]/radian, Vector3f::UnitX())
		  *AngleAxisf(ZXY[2]/radian,  Vector3f::UnitY()) ;
		result = Quaternionf(rot);
		return result;
	}
	Vector3f root_position;
	Quaternionf root_ori;
	std::vector<Quaternionf ,Eigen::aligned_allocator<Quaternionf> > non_root;
};
class MotionCalc{
public:

	glm::vec3 E2g(Vector3f v){glm::vec3 result;
	result[0] = v(0);	result[1] = v(1);	result[2] = v(2);
	return result;
	}
	Vector3f g2E(glm::vec3 v){Vector3f result;
	result(0) = v[0];result(1) = v[1];result(2)=v[2];
	return result;
	}
	Quaternionf glmEuler2EigenQuat(glm::vec3 ZXY);
	void Alignment(motion_vec motionA, motion_vec motionB);
	void warping(motion_vec motionA, motion_vec motionB);
	float scalar_trans(int current,int start, int end);
	void change_motion(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step);
	void Align_motion(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step);
	void Align_motion_at(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step);
	Vector3f root_pos_diff;
	Quaternionf root_ori_diff;
	std::vector<Quaternionf> non_root_diff;
};

class Blending{//항상 B1 이 기준이며 B2 를 B1에 맞추는 쪽으로
public:
	double distance(glm::vec3* B1, glm::vec3* B2,int size);
	void matching(Body_structure& B1, Body_structure& B2);
	double Dynamic(int i, int j);
	void make_pair(int i, int j);
	void align_traj(Body_structure &B1_ori,Body_structure &B2_ori);


	Body_structure Blend_Motion(Body_structure B1_ori,Body_structure B2_ori,double w1);
	Body_structure Blend_at_moment(Body_structure B1_ori,Body_structure B2_ori,double w1);
	std::vector<std::pair<int,int> > corres;
	std::vector<int> M1_pair_Index;
	std::vector<int> M2_pair_Index;
	MatrixXf distance_mat;
	MatrixXf M_;
	int M1_start_frame;
	int M2_start_frame;

	motion_vec tool;
};






































#endif /* ANIMATION_PRO_MOTIONCALC_H_ */
