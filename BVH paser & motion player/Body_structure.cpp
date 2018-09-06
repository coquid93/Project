/*
 * Body_structure.cpp
 *
 *  Created on: 2016. 9. 27.
 *      Author: luis
 */
#include "Body_structure.h"
//#include <windows.h>

#include <GL/gl.h>
#include <GL/glut.h>
//#include <ctime>

void Node::set_child_num(int n){
	num_child = n;

	Node *node;
	node = new Node[n];

	for(int i=0; i<n ; i++){

		Child.push_back(node[i]);
	}
}

void Node::set_root(){
	name = "pelvis";
	position = glm::vec3(0,0,0);
	transform = glm::mat4x4(0); //0 Mat
	transform[3][3]=1;
	this->set_rotation(glm::vec3(0,0,1),0);
}
void Node::draw_node(){
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1,1,1);
//	if(name !="pelvis"){
//	draw_bone();
//	}

	//glutWireCone(0.1,1,10,10);

	//glRotatef(angle,axis[0],axis[1],axis[2]);
	glm::mat4x4 m2 = get_rotation_mat();
	GLdouble m_r[16];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
		m_r[i*4+j] = m2[i][j];
		//std::cout<<m_r[i+j*4]<<" ";
		}
		//std::cout<<std::endl;
	}



	if(name !="pelvis"){
	draw_cone();
	}
	glTranslatef(get_trans_vector()[0],get_trans_vector()[1],get_trans_vector()[2]);
	glutWireSphere(2,10,10);
	glMultMatrixd(m_r);
	if(num_child >0){
		for(int i =0;i<Child.size();i++){
			Child[i].draw_node();
			//std::cout<<"call"<<std::endl;

		}
	}
	glPopMatrix();

}
//animation core part
void Node::draw_cone(){

	//std::cout<<get_trans_vector()[0]<<" "<<get_trans_vector()[1]<<" "<<get_trans_vector()[2]<<" "<<std::endl;
	glm::vec3 z_axis(0,0,1);
	float angle = glm::acos(glm::dot(z_axis,get_trans_vector())/(length(z_axis)*length(get_trans_vector())));
	//std::cout<<angle<<std::endl;

	angle *=57.2957;
	glm::vec3 rot_axis = glm::cross(z_axis,get_trans_vector());
	//std::cout<<rot_axis[0]<<rot_axis[1]<<std::endl;

	glPushMatrix();
	glTranslatef(get_trans_vector()[0]/2,get_trans_vector()[1]/2,get_trans_vector()[2]/2);
	glRotatef(angle,rot_axis[0],rot_axis[1],rot_axis[2]);



	glScalef(2,2,glm::length(get_trans_vector())/2-2);

	//std::cout<<glm::length(get_trans_vector())<<std::endl;
	glutWireSphere(1,10,10);

//	glTranslatef(0,0,0.2);
//	glutWireCone(0.15,glm::length(get_trans_vector())-0.4,10,10);
	glPopMatrix();
}

void Node::make_child(std::string name_tmp,glm::vec3 v){
	Node new_child;
	new_child.parent = this;
	new_child.set_name(name_tmp);
	new_child.set_position(v);

	glm::vec3 tran =v- this->get_position();
	new_child.transform = glm::translate(new_child.transform,tran);
	new_child.set_rotation(glm::vec3(0,0,1),0);
	Child.push_back(new_child);
	num_child++;


}
void Node::set_rotation(glm::vec3 v,float angle){
	this->angle = angle;
	this->axis = v;
}
void Node::set_transform(glm::vec3 tran){
	transform[3][0]=tran[0];
	transform[3][1]=tran[1];
	transform[3][2]=tran[2];

}
void Body_structure::generate_Luis_body(){ //make my Body model.
	root.set_root(); //make root -> pelvis
	root.make_child("lFemur",glm::vec3(-0.8,-0.5,0));
	root.make_child("RFemur",glm::vec3(0.8,-0.5,0));
	root.make_child("thorax",glm::vec3(0,2.3,0));
	//set pos

	current_node = &(root.Child[0]);
	current_node->make_child("lTibia",glm::vec3(-1,-2,0));

	current_node = &(current_node->Child[0]);
	current_node->make_child("lFoot",glm::vec3(-1,-3.5,0));

	current_node = &(current_node->Child[0]);
	current_node->make_child("lToes",glm::vec3(-1.3,-3.5,0));
	current_node = &(current_node->Child[0]);
	current_node->set_rotation(glm::vec3(0,1,0),90);


	//

	current_node = &(root.Child[1]);
	current_node->make_child("rTibia",glm::vec3(1,-2,0));

	current_node = &(current_node->Child[0]);
	current_node->make_child("rFoot",glm::vec3(1,-3.5,0));

	current_node = &(current_node->Child[0]);
	current_node->make_child("rToes",glm::vec3(1.3,-3.5,0));
	current_node = &(current_node->Child[0]);
	current_node->set_rotation(glm::vec3(0,1,0),-90);



	current_node = &(root.Child[2]);
	current_node->set_transform(glm::vec3(0,1.5,0));

	current_node->make_child("Head",glm::vec3(0,2.6,0));
	current_node->make_child("lclavicle",glm::vec3(-0.5,2.5,0));
	current_node->make_child("rclavicle",glm::vec3(0.5,2.5,0));

	current_node = &(current_node->Child[0]);
	current_node->make_child("head_end",glm::vec3(0,2.8,0));

	current_node = &(root.Child[2].Child[1]);
	current_node->make_child("lhumerus",glm::vec3(-1.6,2.5,0));
	current_node = &(current_node->Child[0]);
	current_node->make_child("lradius",glm::vec3(-2.6,2.5,0));
	current_node = &(current_node->Child[0]);
	current_node->make_child("lhand",glm::vec3(-2.8,2.5,0));



	current_node = &(root.Child[2].Child[2]);
	current_node->make_child("rhumerus",glm::vec3(1.6,2.5,0));
	current_node = &(current_node->Child[0]);
	current_node->make_child("rradius",glm::vec3(2.6,2.5,0));
	current_node = &(current_node->Child[0]);
	current_node->make_child("rhand",glm::vec3(2.8,2.5,0));


}
void Body_structure::update_angle(float s){
	root.set_transform(glm::vec3(glm::sin(s/2),glm::sin(s/2),glm::sin(s/2)));
	current_node = &(root.Child[0]);//lFemur
	current_node->set_rotation(glm::vec3(1,0,0),-20*(glm::sin(s/2)));

	current_node = &(current_node->Child[0]); //ltibia
	current_node->set_rotation(glm::vec3(1,0,0),60*glm::sin(s/2));
	current_node = &(current_node->Child[0]); //lFoot
	current_node->set_rotation(glm::vec3(1,0,0),60*(glm::sin(s/2)));

	//
	current_node = &(root.Child[1]);
	current_node->set_rotation(glm::vec3(1,0,0),20*glm::sin(s/2));
	current_node = &(current_node->Child[0]);//rtibia
	current_node->set_rotation(glm::vec3(1,0,0),60*(-glm::sin(s/2)));

	current_node = &(current_node->Child[0]);//rFoot
	current_node->set_rotation(glm::vec3(1,0,0),60*(-glm::sin(s/2)));

	current_node = &(root.Child[2]);
	current_node->set_rotation(glm::vec3(2,1,0),20*(-glm::sin(s/2)));
	current_node = &(current_node->Child[0]);//head
	current_node->set_rotation(glm::vec3(0,0,1),20*(-glm::sin(s/2)));

	current_node = &(root.Child[2].Child[1]);//lclavicle
	current_node->set_rotation(glm::vec3(0,1,0),40*(-glm::sin(s/2)));

	current_node = &(current_node->Child[0]);//lhumerus
	current_node->set_rotation(glm::vec3(0,1,0),40*(-glm::sin(s/2)));

	current_node = &(current_node->Child[0]);//lradius
	current_node->set_rotation(glm::vec3(0,0,1),20*(-glm::sin(s/2)));




	current_node = &(root.Child[2].Child[2]);//rclavicle
	current_node->set_rotation(glm::vec3(0,1,0),-40*(glm::sin(s/2)));
	current_node = &(current_node->Child[0]);//rhumerus
	current_node->set_rotation(glm::vec3(1,0,0),40*(-glm::sin(s/2)));
	current_node = &(current_node->Child[0]);//rradius
	current_node->set_rotation(glm::vec3(0,0,1),30*(-glm::sin(s/2)));


}
//Node Body_structure::find_node(std::string n){
////	if(n =="pelvis")
////		return root;
////	current_node = &(root);
////	while(current_node->get_child_num()>0){
////		for(int i=0;i<current_node->get_child_num();i++){
////			if(current_node->name == n){
////				//return &current_node;
////			}
////
////		}
////	}
//}

std::string Body_structure::get_Node_name(Node n){
	return n.get_name();
}

std::string Node::get_name(){
	return name;
}
