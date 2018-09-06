/*
 * Body_structure.h
 *
 *  Created on: 2016. 9. 27.
 *      Author: luis
 */

#ifndef BODY_STRUCTURE_H_
#define BODY_STRUCTURE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

#include <vector>
#include <string>

class Node{
public:
	Node(){ num_child = 0;}
	virtual ~Node() {}
	void clear_data();
	void set_root();
	void set_position(glm::vec3 pos) {position = pos;}

	bool is_root();
	bool is_leaf();


	void set_child_num(int num);
	void set_name(std::string name_tmp){name = name_tmp;}
	void make_child(std::string name_tmp, glm::vec3 v);
	void make_child_bvh(std::string name);
	int get_child_num(){return num_child;}
	Node get_parent();

	Node* find_get_child(std::string name_);

	std::string get_name();

	glm::vec3 get_position(){ return position;}
	glm::vec3 get_trans_vector(){return glm::vec3(transform[3][0],transform[3][1],transform[3][2]);}

	//define rotation & transformation
	void set_initial_mat(Node root);
	void set_rotation(glm::vec3 axis, float angle);
	void set_transform(glm::vec3 trans);

	void set_rotation_mat(glm::mat4x4 m_){rotation = m_;}
	glm::mat4x4 get_rotation_mat(){return rotation;}

	//draw
	void draw_node();
	void draw_bone();
	void draw_cone();

	std::vector<Node> Child;
	Node *parent;
	glm::vec3 axis;
	float angle;


	int channel_index;
private:
	glm::vec3 position;


	std::string name;
	int num_child;
	glm::mat4x4 transform;
	glm::mat4x4 rotation;


};


class Body_structure {
public:
	Body_structure(){ std::cout<<"Body_structure"<<std::endl;}
	virtual ~Body_structure () {}
	void clear_data();
	void generate_Luis_body();
	void update_angle(float s);
	void update_by_bvh(int frame,Node* n);

	Node find_node(std::string name_n);
	void set_matrix(Node root);
	void draw_body();
	std::string get_Node_name(Node n);
	Node root;
	//Node *body_node;
	Node *current_node;
	std::vector<glm::vec3> root_pos_;
	std::vector<glm::vec3*> motion_ZXY_;

	glm::mat4x4 make_rot_mat(glm::vec3 euler);
private:

};

class Read_BVH{
public:
	Read_BVH(){}
	void clear_data();
	Body_structure body_struct;
	void read_data(std::string filename);
	void Create_body(Body_structure& B);
	int num_frame;
	double frame_time;
	Node root_bvh;
	//Node *body_node_bvh;
	Node *current_node_bvh;
	int num_channel;
	int level;
	std::vector<glm::vec3> root_pos;
	std::vector<glm::vec3*> motion_ZXY;

};


































///////////////////////////////////////////////

//class Bone{
//
//};
//class Joint{
//public:
//	glm::vec3 joint_pos;
//private:
//
//};
//class Body{
//
//};



#endif /* BODY_STRUCTURE_H_ */
