/*
 * picking.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: luislee
 */




#include "../include/picking.h"


using namespace Eigen;


Matrix4d GL2Eigen(GLdouble *mat){
	Matrix4d m;
	m.col(0) = Vector4d(mat[0],mat[1],mat[2],mat[3]);
	m.col(1) = Vector4d(mat[4],mat[5],mat[6],mat[7]);
	m.col(2) = Vector4d(mat[8],mat[9],mat[10],mat[11]);
	m.col(3) = Vector4d(mat[12],mat[13],mat[14],mat[15]);
	return m;
}
GLdouble* Eigen2GL(Matrix4d mat){
	GLdouble* m;
	m = new GLdouble[16];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[4*i+j] = mat(i,j);
		}
	}
	return m;
}
void Picking::hit_update(){

	GLdouble *pj_mat = new GLdouble[16];
	GLdouble *v_mat = new GLdouble[16];
	GLint *identity = new GLint[4];
	GLdouble xx,yy,zz;
	glGetDoublev(GL_PROJECTION_MATRIX, pj_mat);
	glGetDoublev(GL_MODELVIEW_MATRIX, v_mat);
	glGetIntegerv(GL_VIEWPORT,identity);
	Eigen::Matrix4d projection = GL2Eigen(pj_mat);
	Eigen::Matrix4d modelview = GL2Eigen(v_mat);
	if(pick_button){
		modelview_mat = modelview;
		projection_mat = projection;
		pick_cal();
		hit_check();
	}
	else{
		for (int i = 0; i < item.size(); i++) {
				if(pick_button == false){
				item[i]->hit = false;	moving = false;
			}
		}
	}
}
void Picking::get_mouse(Vector2d viewport,Vector3d mouse_pos){
	normalized_pos = Vector3d(mouse_pos(0)/viewport(0)*2-1,-(mouse_pos(1)/viewport(1)*2-1),0);
}
void Picking::pick_cal(){
	Matrix4d inv_pj = projection_mat.inverse();
	Matrix4d inv_mv = modelview_mat.inverse();
	Vector4d temp_near = (inv_pj*Vector4d(normalized_pos(0),normalized_pos(1),0,1));
	Vector4d temp_far = (inv_pj*Vector4d(normalized_pos(0),normalized_pos(1),1,1));

	temp_near = temp_near/temp_near(3);
	temp_far = temp_far/temp_far(3);
	temp_near = inv_mv*temp_near;
	temp_far = inv_mv*temp_far;
	near__ = Vector3d(temp_near(0),temp_near(1),temp_near(2));
	far__ = Vector3d(temp_far(0),temp_far(1),temp_far(2));
}

struct sort_pred {
    bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
        return left.second < right.second;
    }
};

void Picking::hit_check(){
	//assume line through near__ and far__ point
	//far__과 near__를 잇는 라인의 vector
	Vector3d v_line = far__-near__;
	//geometry의 중점과 near__의 vector

	for (int i = 0; i < item.size(); i++) {
		if(item[i]->hit == true) return;
	}

	for (int i = 0; i < item.size(); ++i) {
		item[i]->hit = false;
	}
	std::vector<std::pair<int,float> > prior;
	if(pick_button){
		for (int i = 0; i < item.size(); i++) {

			Vector4d temp = item[i]->orientation.block<1,4>(3,0);
			Vector3d v_center2near = Vector3d(temp(0),temp(1),temp(2))-near__;
//			std::cout<<"center ="<<temp.transpose()<<std::endl;
//			std::cout<<"v_center2near = "<<v_center2near.transpose()<<std::endl;
//			std::cout<<"v_line = "<<v_line.transpose()<<std::endl;
			double distance = (v_line.cross(v_center2near)).norm();
//			std::cout<<distance<<std::endl;
			distance = distance/v_line.norm();
			if(distance < item[i]->radius){
				Vector4d z_ = modelview_mat*temp;
				z_ = z_/z_(3);
				z_ = projection_mat*z_;
				std::pair<int,float> p;
				p.first = i; p.second = z_(2);
				prior.push_back(p);
//				item[i]->hit = true;	moving = true;
//				break;
			}
		}
	}
	if(prior.size()>0){

		std::sort(prior.begin(), prior.end(), sort_pred());
		for(int i=0;i<prior.size();i++){
			std::cout<<prior[i].second<<std::endl;
		}
		std::cout<<"\n";
		item[prior[0].first]->hit = true; moving = true;
	}
	prior.clear();
}
void Picking::move_object(Vector3d mouse_move){
	//프로젝션된 후의 z 컴포넌트를 찾자.
	for (int i = 0; i < item.size(); ++i) {
		if(item[i]->hit == true){
			Vector4d o_pos = item[i]->orientation.block<1,4>(3,0);
			Vector4d z_plane = item[i]->orientation.block<1,4>(3,0);
			z_plane = projection_mat*modelview_mat*z_plane;
			z_plane += Vector4d(mouse_move(0),mouse_move(1),0,0)*z_plane(3);
			z_plane = modelview_mat.inverse()*projection_mat.inverse()*z_plane;
			move_direction = z_plane - o_pos;
			Vector3d dxyz = Vector3d(move_direction(0),move_direction(1),move_direction(2));
			item[i]->trans_geo(dxyz);
			return;
		}
	}
}












