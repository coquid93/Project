/*
 * IKsolver.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: luislee
 */


#include "Body_structure.h"
#include <Eigen/SVD>
using namespace std;
using namespace glm;
using namespace Eigen;

template<typename _Matrix_Type_>
_Matrix_Type_ pseudoInverse_(const _Matrix_Type_ &a, double epsilon = std::numeric_limits<double>::epsilon())
{
	Eigen::JacobiSVD< _Matrix_Type_ > svd(a ,Eigen::ComputeThinU | Eigen::ComputeThinV);
	double tolerance = epsilon * std::max(a.cols(), a.rows()) *svd.singularValues().array().abs()(0);
	return svd.matrixV() *  (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().array().inverse(), 0).matrix().asDiagonal() * svd.matrixU().adjoint();
}

void IKsolver::compute_jacobian(Node* end){

}
void IKsolver::compute_delta_end(Eigen::MatrixXf &m,Node* end){
	Vector3f d_v = glm2eigen_vec3( end->target_global_pos-end->global_pos);
	Vector3f end_ = glm2eigen_vec3( end->global_pos);
	cout<<"distance : "<<end_.transpose()<<endl;
	Vector3f d_w ;
	d_w.setZero();
	m.block(0,0,3,1) = d_v;
	m.block(3,0,3,1) = d_w;
	return ;
}
int IKsolver::compute_jacobian_size(Node* end_eff){
	int size_  = 0;
	Node* curr = end_eff;
	while(curr->channel_index > 0 ){
		size_++;

		curr = curr->parent;//	cout<<new_child.get_name()<<" 's parent = "<<new_child.parent->get_name()<<endl;

		//cout<<curr->get_name()<<endl;
	}
	return size_;
}
void Body_structure::update_by_IK(Node *root){
	Node* target = &(root->Child[2]);
	target = &(target->Child[1]);
	target = &(target->Child[0]);
	target = &(target->Child[0]);
	target = &(target->Child[0]);
	Node* end = &(root->Child[2]);
	end = &(end->Child[1]);
	end = &(end->Child[0]);
	end = &(end->Child[0]);
	end = &(end->Child[0]);

	Node * end_eff = end;
	end->set_global_pos();
	end->target_global_pos = glm::vec3(31.5555, 86.607, -95.8799);
	glm::vec3 target_direction = end->global_pos-end->target_global_pos; // 31.5555 , 86.607, -98.8799
	IKsolver IK;
	//cout<<"distance \n"<< IK.glm2eigen_vec3(target_direction).transpose()<<endl;
	if(glm::length(target_direction)>0.1){
		int num_channel = IK.compute_jacobian_size(end_eff);
		MatrixXf J(6,num_channel*3);
		for(int i = 0; i<num_channel;i++){
			target = target->parent;
			int ind = i*3;
			MatrixXf block_(6,3);
			IK.compute_block(block_,target,end_eff);
			J.block(0,ind,6,3) = block_;
		}
		MatrixXf j_pinv = pseudoInverse_(J);
		MatrixXf d_end(6,1); // compute it
		IK.compute_delta_end(d_end,end_eff);
		cout<<J<<endl;
		MatrixXf d_theta; // unknown

		d_theta = j_pinv*d_end;

//		JacobiSVD<MatrixXf> svd(J, ComputeThinU | ComputeThinV);
//		d_theta = svd.solve(d_end);

//		d_theta = J.householderQr().solve(d_end);

		//cout<<j_pinv.row(0)<<endl;

		//cout<<d_theta<<endl;
		cout<<IK.glm2eigen_rotation(end_eff->global_tranform)<<endl;
		cout<<"Endeff "<<IK.glm2eigen_vec3( end_eff->global_pos).transpose()<<endl;
		cout<<"parent "<<IK.glm2eigen_vec3( end_eff->parent->global_pos).transpose()<<endl;

		update_ZXY_IK(d_theta*0.1, end_eff);

		//cout<<d_end - J*d_theta<<endl;
		update_by_bvh(int(frame_step),root);
	}
}
void Body_structure::update_ZXY_IK(MatrixXf d_theta , Node* end){
	Node * current_ = end->parent;
	//cout<<end->global_pos[0]<<" , "<<end->global_pos[1]<<" , "<<end->global_pos[2]<<endl;
	motion_ZXY_[frame_step][current_->channel_index][1] += 10;//x
	std::cout<<current_->get_name()<<endl;
//	for(long int i = 0 ; i<5 ; i++){
//		if(current_->get_name() == "pelvis")
//			continue;
//		int channel_idx = current_->channel_index;
//		//cout<<channel_idx<<" - "<<motion_ZXY_[frame_step][channel_idx][0]<<" to ";
//		motion_ZXY_[frame_step][channel_idx][0] += d_theta(i*3+2,0);//z
//		//cout<<motion_ZXY_[frame_step][channel_idx][0]<<endl;
//		motion_ZXY_[frame_step][channel_idx][1] += d_theta(i*3+0,0);//x
//		motion_ZXY_[frame_step][channel_idx][2] += d_theta(i*3+1,0);//y
//		//motion_ZXY_[frame_step][channel_idx][1] += 1;//x
//		current_ = current_->parent;
//	}
	return;
}
void IKsolver::compute_block(MatrixXf &m, Node* target, Node* end){
	Matrix3f v_m , v_w;
	v_m = compute_v_mat(target, end).transpose();
	v_w = compute_w_mat(target);
	m.block(0,0,3,3) = v_m;
	m.block(3,0,3,3) = v_w; // rotation matrix 저장
//	cout<<m<<endl;
	return;
}

Matrix3f IKsolver::compute_v_mat(Node* pivot, Node* end){
	Matrix3f result;
	Matrix3f w = compute_w_mat(end);
	Vector3f w_x = 	w.col(0);
	Vector3f w_y =	w.col(1);
	Vector3f w_z =	w.col(2);
	pivot->set_global_pos();
	end->set_global_pos();
	Eigen::Vector3f p_vec = glm2eigen_vec3( end->global_pos-pivot->global_pos);
	//cout<<p_vec<<endl;
	result.col(0) = w_x.cross(p_vec);
	result.col(1) = w_y.cross(p_vec);
	result.col(2) = w_z.cross(p_vec);
	return result;

}
Matrix3f IKsolver::compute_w_mat(Node* n){
	glm::mat4x4 m = n->global_tranform;
	glm::transpose(m);

	Matrix3f result;
	result(0,0) = m[0][0];
	result(0,1) = m[0][1];
	result(0,2) = m[0][2];
	result(1,0) = m[1][0];
	result(1,1) = m[1][1];
	result(1,2) = m[1][2];
	result(2,0) = m[2][0];
	result(2,1) = m[2][1];
	result(2,2) = m[2][2];
	return result.transpose();
}
Matrix3f IKsolver::glm2eigen_rotation(glm::mat4x4 m){
	glm::transpose(m);
	Matrix3f result;
	result(0,0) = m[0][0];
	result(0,1) = m[0][1];
	result(0,2) = m[0][2];
	result(1,0) = m[1][0];
	result(1,1) = m[1][1];
	result(1,2) = m[1][2];
	result(2,0) = m[2][0];
	result(2,1) = m[2][1];
	result(2,2) = m[2][2];
	return result;
}
Eigen::Vector3f IKsolver::glm2eigen_vec3(glm::vec3 v){
	Eigen::Vector3f eigen;
	eigen(0) = v[0];
	eigen(1) = v[1];
	eigen(2) = v[2];
	return eigen;
}

template<typename _Matrix_Type_>
bool pseudoInverse(const _Matrix_Type_ &a, _Matrix_Type_ &result, double epsilon = std::numeric_limits<typename _Matrix_Type_::Scalar>::epsilon())
{
  if(a.rows()<a.cols())
      return false;

  Eigen::JacobiSVD< _Matrix_Type_ > svd = a.jacobiSvd();

  typename _Matrix_Type_::Scalar tolerance = epsilon * std::max(a.cols(), a.rows()) * svd.singularValues().array().abs().maxCoeff();

  result = svd.matrixV() * _Matrix_Type_(_Matrix_Type_( (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().
      array().inverse(), 0) ).diagonal()) * svd.matrixU().adjoint();
}


