/*
 * Blending.cpp
 *
 *  Created on: Dec 1, 2016
 *      Author: luislee
 */
#include "MotionCalc.h"

using namespace std;
//size = channel num
double Blending::distance(glm::vec3* B1, glm::vec3* B2,int size){
	double result =0;
	for(int i=1;i<size; i++){//root ori 는 제외
		Quaternionf B1_quat = tool.euler2quat(B1[i]);
		Quaternionf B2_quat = tool.euler2quat(B2[i]);
		double angular = ((B1_quat.inverse()*B2_quat).w());
		angular = glm::acos(angular)*2;
		result += angular*angular;
	}
	return result;
}

double Blending::Dynamic(int i, int j){
	//cout<<i<<" , "<<j<<endl;
	if(i==0&&j==0) { 	M_(0,0) = distance_mat(0,0); return M_(0,0); 					}
	if(i==1&&j==0) {	M_(i,j) = distance_mat(i,j)+distance_mat(0,0); return M_(i,j);	}
	if(i==0&&j==1) {	M_(i,j) = distance_mat(i,j)+distance_mat(0,0); return M_(i,j);	}
	if(i==0&&j>0)  {	M_(i,j) = distance_mat(i,j)+Dynamic(i,j-1); return M_(i,j);		}
	if(j==0&&i>0)  {	M_(i,j) = distance_mat(i,j)+Dynamic(i-1,j); return M_(i,j);		}
	double a,b,c,min1,min2;
	if(M_(i,j) <0){
		a = Dynamic(i-1,j);
		b = Dynamic(i,j-1);
		c = Dynamic(i-1,j-1);
		M_(i-1,j) = a;
		M_(i,j-1) = b;
		M_(i-1,j-1) =c;

	if(a>b)		{ 	min1 = b;	}
	else 		{	min1 = a;	}
	if(min1 >c) {	min2 = c;	}
	else		{	min2 = min1;}
	if(i==1&&j==1)	cout<<"min2 = "<<min2<<" , "<<a<<" , "<<b<<" , "<<c<<endl;
	double result =  min2 + distance_mat(i,j);
	M_(i,j) = result;
	}
	return M_(i,j);
}
void Blending::make_pair(int i, int j){

	corres.push_back(std::make_pair(i+M1_start_frame,j+M2_start_frame));
	if(i==0&&j==0)return;
	else if(i==0&&j!=0)make_pair(0,j-1);
	else if(i!=0&&j==0)make_pair(i-1,0);
	else{
		double a,b,c;
		a= M_(i-1,j);
		b= M_(i,j-1);
		c= M_(i-1,j-1);
		if	   (a<=b&& a<=c)make_pair(i-1,j);
		else if(b<=a&&b<=c)make_pair(i,j-1);
		else if(c<=b&&c<=a)make_pair(i-1,j-1);
		else{
			cout<<"Error!"<<endl;
		}
	}
}
void Blending::matching(Body_structure& B1,Body_structure& B2){
	M1_start_frame = B1.edit_point[0]; M2_start_frame = B2.edit_point[0];
	int B1_size = B1.edit_point[1]-B1.edit_point[0]+1;
	int B2_size = B2.edit_point[1]-B2.edit_point[0]+1;
	distance_mat.resize(B1_size,B2_size);
	M_.resize(B1_size,B2_size);
	M_.setConstant(-1);
	int channel_size = B1.num_channel;
	for(int i=B1.edit_point[0] ;i<B1.edit_point[1]+1; i++){
		for(int j=B2.edit_point[0] ; j<B2.edit_point[1]+1; j++){
			distance_mat(i-B1.edit_point[0],j-B2.edit_point[0])
					= distance(B1.motion_ZXY_ori[i],B2.motion_ZXY_ori[j],channel_size);
		}
	}
	Dynamic(B1_size-1,B2_size-1);
	make_pair(B1_size-1,B2_size-1);

}
void Blending::align_traj(Body_structure &B1_ori,Body_structure &B2_ori){//start frame 저장 후 call
	//step1 : root orientation 일치
	M1_start_frame = B1_ori.edit_point[0];
	M2_start_frame = B2_ori.edit_point[0];
	MotionCalc align;
	align.Align_motion(B1_ori,M1_start_frame,B2_ori,M2_start_frame);
//now B2 body 는 변경된 상태이므로 B2 에 대해서는 original 데이터르 쓰면 안됨.

	//step2 : root trajactory (velocity 이용)

}
//w1 = B1 의 weight 임 B1 의 num_frame 이 더 커야함
Body_structure Blending::Blend_Motion(Body_structure B1_ori,Body_structure B2_ori,double w1){
	MotionCalc M_tool;
	Body_structure Result , B1, B2; // Motion_ZXY 가 포인터라서 연결을 끊는 방법 구현 : pointer 하나 더만들ㅇ서 그거로 교체한다.
//	B1 = B1_ori;
//	cout<<&B1 << ", "<< &B1_ori<<endl;
//	Result = B1;
	B1=B1_ori; B2 = B2_ori; Result = B1;
	B1.break_chain(B1);
	B2.break_chain(B2);
	Result.break_chain(Result);
	//
	matching(B1,B2);
	align_traj(B1,B2);
	//B1, B2 변경됨
	int B1_clipSize = B1.edit_point[1]-B1.edit_point[0]+1;
	int B2_clipSize = B2.edit_point[1]-B2.edit_point[0]+1;
	//진짜 Blending step//
	static int past = -1;
	for (int i=0;i<corres.size();i++){
		for(int j=0;j<B1.num_channel;j++){
			//if(past == corres[i].first) continue;
			Quaternionf B1_quat = tool.euler2quat(B1.motion_ZXY_[corres[i].first][j]) ;
			Quaternionf B2_quat = tool.euler2quat(B2.motion_ZXY_[corres[i].second][j]);
			Quaternionf Result_quat = B1_quat.slerp(1-w1,B2_quat);
			Vector3f v = Result_quat.toRotationMatrix().eulerAngles(2,0,1) * radian;
			Quaternionf compare = tool.euler2quat(M_tool.E2g(v));
			Result.motion_ZXY_[corres[i].first][j] = M_tool.E2g(v);
			Result.motion_ZXY_ori[corres[i].first][j] = M_tool.E2g(v);
			past = corres[i].first;

		}
	}
	for (int i=corres.size()-1;i>0;i--){
		Quaternionf B1_quat = tool.euler2quat(B1.motion_ZXY_[corres[i].first][0]) ;
		Quaternionf B2_quat = tool.euler2quat(B2.motion_ZXY_[corres[i].second][0]);
		Vector3f B1_root_pos = M_tool.g2E(B1.root_pos_[corres[i].first]);
		Vector3f B1_root_pos_next = M_tool.g2E(B1.root_pos_[corres[i-1].first]);
		Vector3f B2_root_pos = M_tool.g2E(B2.root_pos_[corres[i].second]);
		Vector3f B2_root_pos_next = M_tool.g2E(B2.root_pos_[corres[i-1].second]);
		Quaternionf Result_root_ori = tool.euler2quat(Result.motion_ZXY_[corres[i].first][0]);
		Vector3f B1_diff = B1_root_pos_next-B1_root_pos;
		Vector3f B2_diff = B2_root_pos_next-B2_root_pos;
		Vector3f B1_diff_local = B1_quat.inverse()._transformVector(B1_diff);
		Vector3f B2_diff_local = B2_quat.inverse()._transformVector(B2_diff);
		float B1_length = B1_diff_local.norm();
		float B2_length = B2_diff_local.norm();
		if(B1_diff.norm() == 0) {B1_diff_local = Eigen::Vector3f(0,0,0);}
		if(B2_diff.norm() == 0) {B2_diff_local = Eigen::Vector3f(0,0,0);}
		float Result_length = (B1_length*w1+B2_length*(1-w1));
		Vector3f Result_vec = B1_diff_local*w1 +B2_diff_local*(1-w1) ;
		Result_vec.normalize();
		Result_vec = Result_vec * Result_length;
		Result_vec = Result_root_ori._transformVector(Result_vec);
		Result.root_pos_[corres[i-1].first] = Result.root_pos_[corres[i].first] + M_tool.E2g(Result_vec);
		Result.root_pos_ori[corres[i-1].first] = Result.root_pos_[corres[i].first] ;
	}

	return Result;
}

Body_structure Blending::Blend_at_moment(Body_structure B1_ori,Body_structure B2_ori,double w1){
	MotionCalc M_tool;
	Body_structure Result , B1, B2; // Motion_ZXY 가 포인터라서 연결을 끊는 방법 구현 : pointer 하나 더만들ㅇ서 그거로 교체한다.
	B1=B1_ori; B2 = B2_ori; Result = B1;
	B1.break_chain(B1);
	B2.break_chain(B2);
	Result.break_chain(Result);
	matching(B1,B2);
	int index ;
	for (int i = 0; i < corres.size(); i++) {
		if(corres[i].first == B1_ori.frame_step){
			index = i; break;
		}
	}
	static int past = -1;
	for (int i=0;i<index+1;i++){
		for(int j=0;j<B1.num_channel;j++){
			//if(past == corres[i].first) continue;
			Quaternionf B1_quat = tool.euler2quat(B1.motion_ZXY_[corres[i].first][j]) ;
			Quaternionf B2_quat = tool.euler2quat(B2.motion_ZXY_[corres[i].second][j]);
			Quaternionf Result_quat = B1_quat.slerp(1-w1,B2_quat);
			Vector3f v = Result_quat.toRotationMatrix().eulerAngles(2,0,1) * radian;
			Quaternionf compare = tool.euler2quat(M_tool.E2g(v));
			Result.motion_ZXY_[corres[i].first][j] = M_tool.E2g(v);
			Result.motion_ZXY_ori[corres[i].first][j] = M_tool.E2g(v);
			past = corres[i].first;

		}
	}
	for (int i=index;i>0;i--){
		Quaternionf B1_quat = tool.euler2quat(B1.motion_ZXY_[corres[i].first][0]) ;
		Quaternionf B2_quat = tool.euler2quat(B2.motion_ZXY_[corres[i].second][0]);
		Vector3f B1_root_pos = M_tool.g2E(B1.root_pos_[corres[i].first]);
		Vector3f B1_root_pos_next = M_tool.g2E(B1.root_pos_[corres[i-1].first]);
		Vector3f B2_root_pos = M_tool.g2E(B2.root_pos_[corres[i].second]);
		Vector3f B2_root_pos_next = M_tool.g2E(B2.root_pos_[corres[i-1].second]);
		Quaternionf Result_root_ori = tool.euler2quat(Result.motion_ZXY_[corres[i].first][0]);
		Vector3f B1_diff = B1_root_pos_next-B1_root_pos;
		Vector3f B2_diff = B2_root_pos_next-B2_root_pos;
		Vector3f B1_diff_local = B1_quat.inverse()._transformVector(B1_diff);
		Vector3f B2_diff_local = B2_quat.inverse()._transformVector(B2_diff);
		float B1_length = B1_diff_local.norm();
		float B2_length = B2_diff_local.norm();
		if(B1_diff.norm() == 0) {B1_diff_local = Eigen::Vector3f(0,0,0);}
		if(B2_diff.norm() == 0) {B2_diff_local = Eigen::Vector3f(0,0,0);}
		float Result_length = (B1_length*w1+B2_length*(1-w1));
		Vector3f Result_vec = B1_diff_local*w1 +B2_diff_local*(1-w1) ;
		Result_vec.normalize();
		Result_vec = Result_vec * Result_length;
		Result_vec = Result_root_ori._transformVector(Result_vec);
		Result.root_pos_[corres[i-1].first] = Result.root_pos_[corres[i].first] + M_tool.E2g(Result_vec);
		Result.root_pos_ori[corres[i-1].first] = Result.root_pos_[corres[i].first] ;
	}
	Result.frame_step = corres[index].first;
	return Result;

}











//////////////////


