/*
 * MotionCalc.cpp
 *
 *  Created on: Nov 18, 2016
 *      Author: luislee
 */


#include "MotionCalc.h"


using namespace std;

void MotionCalc::Alignment(motion_vec motionA, motion_vec motionB){
	Vector3f vec_diff = (motionA.root_position - motionB.root_position);
	root_pos_diff = vec_diff;
	root_ori_diff = motionB.root_ori.inverse()*motionA.root_ori;
}
void MotionCalc::warping(motion_vec motionA, motion_vec motionB){
	for(int i=0;i<motionB.non_root.size();i++){
		non_root_diff.push_back(motionB.non_root[i].inverse()*motionA.non_root[i]);
	}
}
void MotionCalc::change_motion(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step){
	motion_vec b1_motion_vec(b1.root_pos_[b1_step],b1.motion_ZXY_[b1_step],b1.num_channel);
	motion_vec b2_motion_vec(b2.root_pos_ori[b2_step],b2.motion_ZXY_ori[b2_step],b2.num_channel);
	//**********************ALIGNMENT***********************//
	Alignment(b1_motion_vec,b2_motion_vec);
	glm::vec3 init_root = b2.root_pos_ori[b2_step];
	glm::vec3 diff_root = E2g(root_pos_diff);
	Vector3f euler_diff = root_ori_diff.toRotationMatrix().eulerAngles(2,0,1);
	for(int i=b2_step;i<b2.edit_point[1]+1;i++){
		Vector3f local_pos = (g2E(b2.root_pos_ori[i]-init_root));
		Vector3f new_pos = root_ori_diff._transformVector(local_pos)+g2E(b1.root_pos_[b1_step]);
		new_pos(1) = b2.root_pos_ori[i][1];
		b2.root_pos_[i] = E2g(new_pos);
		Quaternionf unit;
		Matrix3f m_ = root_ori_diff.toRotationMatrix();
		m_.row(1) = Vector3f(0,1,0);
		//m_.col(1) = Vector3f(0,1,0);
		unit = Quaternionf(m_);
		Quaternionf root_result = root_ori_diff.slerp(1-scalar_trans(i,b2_step,b2.total_frame_num),unit);
		Quaternionf root_q =(glmEuler2EigenQuat(b2.motion_ZXY_ori[i][0])*root_result);
		Vector3f ZXY_result = root_q.toRotationMatrix().eulerAngles(2,0,1) * radian;
		b2.motion_ZXY_[i][0] = E2g(ZXY_result);
	}
	//**********************WARPING***********************//

	warping(b1_motion_vec, b2_motion_vec);

	for(int i=b2_step;i<b2_step+40;i++){
		for(int j=0;j<non_root_diff.size();j++){
			Quaternionf unit;
			unit = unit.Identity();
			Quaternionf result = non_root_diff[j].slerp(1-scalar_trans(i,b2_step,b2.total_frame_num),unit);
			Quaternionf q = (glmEuler2EigenQuat(b2.motion_ZXY_ori[i][j+1])*result);
			Vector3f ZXY_warp =q.toRotationMatrix().eulerAngles(2,0,1);
			ZXY_warp = ZXY_warp*radian;
			b2.motion_ZXY_[i][j+1] = E2g(ZXY_warp);
		}
	}
}
float MotionCalc::scalar_trans(int current,int start,int end){
	//linear
	float x = float(current-start);
	float x_range = float(end-start);
	float y = 1-(x/40);
	if(y<0) y=0;
	if(y>1) y=1;

	return y;
}
Quaternionf MotionCalc::glmEuler2EigenQuat(glm::vec3 ZXY){
	Matrix3f m;
	m = AngleAxisf(ZXY[0]/radian, Vector3f::UnitZ())
	   *AngleAxisf(ZXY[1]/radian, Vector3f::UnitX())
	   *AngleAxisf(ZXY[2]/radian, Vector3f::UnitY());
	return Quaternionf(m);
}

void Body_structure::cliping_frame(int start, int end){
	if(this->frame_step < start){
		this->frame_step = start;
	}
	if(this->frame_step > end){
		this->frame_step = start;
	}
}
//only allignment
void MotionCalc::Align_motion(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step){
	motion_vec b1_motion_vec(b1.root_pos_[b1_step],b1.motion_ZXY_[b1_step],b1.num_channel);
	motion_vec b2_motion_vec(b2.root_pos_ori[b2_step],b2.motion_ZXY_ori[b2_step],b2.num_channel);
	//**********************ALIGNMENT***********************//
	Alignment(b1_motion_vec,b2_motion_vec);
	glm::vec3 init_root = b2.root_pos_ori[b2_step];
	glm::vec3 diff_root = E2g(root_pos_diff);
	Vector3f euler_diff = root_ori_diff.toRotationMatrix().eulerAngles(2,0,1);
	for(int i=b2_step;i<b2.edit_point[1]+1;i++){
		Vector3f local_pos = (g2E(b2.root_pos_ori[i]-init_root));
		Vector3f new_pos = root_ori_diff._transformVector(local_pos)+g2E(b1.root_pos_[b1_step]);
		new_pos(1) = b2.root_pos_ori[i][1];
		b2.root_pos_[i] = E2g(new_pos);
		Quaternionf unit;
		Matrix3f m_ = root_ori_diff.toRotationMatrix();
		m_.col(1) = Vector3f(0,1,0);
		unit = Quaternionf(m_);
		Quaternionf root_result = root_ori_diff.slerp(1-scalar_trans(i,b2_step,b2.total_frame_num),unit);
		Quaternionf root_q =(glmEuler2EigenQuat(b2.motion_ZXY_ori[i][0])*root_result);
		Vector3f ZXY_result = root_q.toRotationMatrix().eulerAngles(2,0,1) * radian;
		b2.motion_ZXY_[i][0] = E2g(ZXY_result);
	}
	for(int i=b1.edit_point[0];i<=b1.edit_point[1];i++){
		b1.root_pos_[i] = b1.root_pos_ori[i];
		for(int j=0;j<b1.num_channel;j++){
			b1.motion_ZXY_[i][j][0] = b1.motion_ZXY_ori[i][j][0];
			b1.motion_ZXY_[i][j][1] = b1.motion_ZXY_ori[i][j][1];
			b1.motion_ZXY_[i][j][2] = b1.motion_ZXY_ori[i][j][2];
		}
	}
}

void MotionCalc::Align_motion_at(Body_structure & b1,int b1_step, Body_structure& b2,int b2_step){
	motion_vec b1_motion_vec(b1.root_pos_[b1_step],b1.motion_ZXY_[b1_step],b1.num_channel);
	motion_vec b2_motion_vec(b2.root_pos_ori[b2_step],b2.motion_ZXY_ori[b2_step],b2.num_channel);
	//**********************ALIGNMENT***********************//
	Alignment(b1_motion_vec,b2_motion_vec);
	warping(b1_motion_vec, b2_motion_vec);

	for(int i=b2_step;i<b2_step+40;i++){
		for(int j=0;j<non_root_diff.size();j++){
			Quaternionf unit;
			unit = unit.Identity();
			Quaternionf result = non_root_diff[j].slerp(1-scalar_trans(i,b2_step,b2.total_frame_num),unit);
			Quaternionf q = (glmEuler2EigenQuat(b2.motion_ZXY_ori[i][j+1])*result);
			Vector3f ZXY_warp =q.toRotationMatrix().eulerAngles(2,0,1);
			ZXY_warp = ZXY_warp*radian;
			b2.motion_ZXY_[i][j+1] = E2g(ZXY_warp);
		}
	}
}











