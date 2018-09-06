/*
 * Read_BVH.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: luislee
 */

#include "Body_structure.h"
#include <string.h>
#include <stdlib.h>

const int MAX_CHARS_PER_LINE = 2048;
const int MAX_TOKENS_PER_LINE = 80;
const char* const DELIMITER = " \t\r\n\f\v";

using namespace std;

void Body_structure::break_chain(Body_structure &B_target){
	cout<<"asd"<<endl;
	for(int i=0;i<B_target.total_frame_num;i++){
		glm::vec3* v_tmp = new glm::vec3[B_target.num_channel];
		glm::vec3* v_tmp_ori = new glm::vec3[B_target.num_channel];
		for(int j=0 ; j<B_target.num_channel;j++){
			v_tmp[j] = B_target.motion_ZXY_[i][j];
			v_tmp_ori[j] = B_target.motion_ZXY_ori[i][j];
		}
		B_target.motion_ZXY_[i] = v_tmp;
		B_target.motion_ZXY_ori[i] = v_tmp_ori;
	}
	return;
}
void Body_structure::clear_data(){
	this->root_pos_.clear();
	this->motion_ZXY_.clear();
}
void Read_BVH::clear_data(){

}

void Node::make_child_bvh(std::string name_){
	Node new_child;
	new_child.parent = this;
	new_child.set_name(name_);
//	cout<<new_child.get_name()<<" 's parent = "<<new_child.parent->get_name()<<endl;
	new_child.set_rotation(glm::vec3(0,0,1),0);
	Child.push_back(new_child);
	num_child++;
	return;
}
Node* Node::find_get_child(std::string name_){
	for(int i=0;i<get_child_num();i++){

		if(this->Child[i].get_name() == name_){
			return &(this->Child[i]);
		}
	}
}

void Read_BVH::read_data(string filename){

	this->data_filename = filename;
	ifstream fin;
	cout<<filename<<endl;
	fin.open(filename.c_str()); // open a file
	if (!fin.good()) {
		  std::cout<<"cannot found file"<<endl;
		  cout<<"Enter filename again : ";
		  cin>>filename;
		  read_data("./BVH_folder/"+filename);
		  return;
	  }
	else{
	  // read each line of the file
		  while (!fin.eof())
		  {
			// read an entire line into memory
			char buf[MAX_CHARS_PER_LINE];
			fin.getline(buf, MAX_CHARS_PER_LINE);

			// parse the line into blank-delimited tokens
			int n = 0; // a for-loop index

			// array to store memory addresses of the tokens in buf
			const char* token[MAX_TOKENS_PER_LINE]={ }; // initialize to 0

			// parse the line

			token[0] = strtok(buf, DELIMITER); // first token
			if (token[0]) // zero if line is blank
			{
			  for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
			  {
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			  }
			}
			if(token[0]){
				//cout<<token[0]<<endl;
				if(strcmp(token[0],"ROOT")==0){
					level = 0;
					//root_bvh.set_name(token[1]);
					root_bvh.set_root();
					root_bvh.channel_index = level;
					current_node_bvh = &root_bvh;
					//current_node_bvh->parent = NULL;
				}
				if(strcmp(token[0],"JOINT")==0 || strcmp(token[0],"End")==0){
					//cout<<token[1]<<endl;

					current_node_bvh->make_child_bvh(token[1]);
					current_node_bvh = current_node_bvh->find_get_child(token[1]);

					if(strcmp(token[0],"JOINT")==0){
						level++;
						current_node_bvh->channel_index = level;
					}
					else
						current_node_bvh->channel_index = 1000;


				}
				if(strcmp(token[0],"{")==0){

					char buf[MAX_CHARS_PER_LINE];
					fin.getline(buf, MAX_CHARS_PER_LINE);
					int n = 0; // a for-loop index
					const char* token[MAX_TOKENS_PER_LINE]={ }; // initialize to 0
					token[0] = strtok(buf, DELIMITER); // first token
					if (token[0]) // zero if line is blank
					{
					  for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
					  {
						token[n] = strtok(0, DELIMITER); // subsequent tokens
						if (!token[n]) break; // no more tokens
					  }
					}
					if(strcmp(token[0],"OFFSET")==0){
						current_node_bvh->set_transform(glm::vec3(atof(token[1]),atof(token[2]),atof(token[3])));
					}

				}
				if(strcmp(token[0],"}")==0){
					cout<<current_node_bvh->get_name()<<endl;
					current_node_bvh = current_node_bvh->parent;
				}
				//	cout<<new_child.get_name()<<" 's parent = "<<new_child.parent->get_name()<<endl;

				if(strcmp(token[0],"MOTION")==0){
					char buf[MAX_CHARS_PER_LINE];
					fin.getline(buf, MAX_CHARS_PER_LINE);
					int n = 0; // a for-loop index
					const char* token[MAX_TOKENS_PER_LINE]={ }; // initialize to 0
					token[0] = strtok(buf, DELIMITER); // first token
					if (token[0]) // zero if line is blank
					{
					  for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
					  {
						token[n] = strtok(0, DELIMITER); // subsequent tokens
						if (!token[n]) break; // no more tokens
					  }
					}
					std::cout<<"frame num = "<<token[1]<<endl;
					num_frame = atoi(token[1]);
					buf[MAX_CHARS_PER_LINE];
					fin.getline(buf, MAX_CHARS_PER_LINE);
					n = 0; // a for-loop index
					token[MAX_TOKENS_PER_LINE]={ }; // initialize to 0
					token[0] = strtok(buf, DELIMITER); // first token
					if (token[0]) // zero if line is blank
					{
					  for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
					  {
						token[n] = strtok(0, DELIMITER); // subsequent tokens
						if (!token[n]) break; // no more tokens
					  }
					}
					if(strcmp(token[0],"Frame")==0){
						cout<<"frame time = "<<token[2]<<endl;
						frame_time = atof(token[2]);
					}

					int frame_num = 0;

					while (frame_num<num_frame)
					{

						char buf[MAX_CHARS_PER_LINE];
						fin.getline(buf, MAX_CHARS_PER_LINE);
						int n = 0; // a for-loop index
						const char* token[MAX_TOKENS_PER_LINE]={ }; // initialize to 0
						token[0] = strtok(buf, DELIMITER); // first token
						if (token[0]) // zero if line is blank
						{
						  for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
						  {
							token[n] = strtok(0, DELIMITER); // subsequent tokens
							if (!token[n]) break; // no more tokens
						  }
						}
						num_channel = n/3-1;
						float offset_pos_x= atof(token[0]);
						float offset_pos_y=atof(token[1]);
						float offset_pos_z=atof(token[2]);
						root_pos.push_back(glm::vec3(offset_pos_x,offset_pos_y,offset_pos_z));
						vector<glm::vec3> v_tmp;
						glm::vec3* v;
						v = new glm::vec3[num_channel];
						for(int i=1;i<num_channel+1; i++){
							v[i-1]= glm::vec3(atof(token[i*3]),atof(token[i*3+1]),atof(token[i*3+2]));
						}
						motion_ZXY.push_back(v);
						frame_num++;

					}//while end
					cout<<frame_num<<endl;

				}

			}

			// process (print) the tokens

		  }
	  }
	  fin.close();
	  std::cout<<"read complete"<<std::endl;
	  cout<<"close"<<endl;
//	  std::cout<<level<<endl;
//	  for(int i=0;i<10;i++){
//		  	  cout<<"root pos = "<<root_pos[i].x<<" "<<root_pos[i].y<<" "<<root_pos[i].z<<endl;
//			 std::cout<<"root rot = "<<motion_ZXY[i][1].x<<" "<<motion_ZXY[i][1].y<<" "<<motion_ZXY[i][1].z<<std::endl;
//
//
//	  }
}




void Read_BVH::Create_body(Body_structure& Body){
	Body.root = root_bvh;

	Body.root_pos_ = root_pos;
	for(int i=0; i<root_pos.size();i++){
		glm::vec3 v = root_pos[i];
		Body.root_pos_ori.push_back(v);
	}

	Body.motion_ZXY_=motion_ZXY;
	for(int i=0; i<motion_ZXY.size();i++){
		glm::vec3* v2;
		v2 = new glm::vec3[num_channel];
		for(int j=0;j<num_channel;j++){
			v2[j] = motion_ZXY[i][j];
		}
		Body.motion_ZXY_ori.push_back(v2);
	}

	Body.total_frame_num = this->num_frame;
	Body.total_frame_time = this->frame_time;
	Body.num_channel = this->num_channel;
}
void Body_structure::update_by_bvh(int frame,Node* n){
	frame = frame % this->total_frame_num;
	if(n->get_name()=="pelvis"){
		n->set_transform(root_pos_[frame]);
		frame_step = frame;
	}


	if(n->get_child_num()>0)
	{
		//cout<<n->get_name()<<" = " <<motion_ZXY_[frame][n->channel_index][0]<<","<<motion_ZXY_[frame][n->channel_index][1]<<endl;
		n->set_rotation_mat(make_rot_mat(motion_ZXY_[frame][n->channel_index]));
		for(int i=0;i<n->get_child_num();i++)
		{
			//cout<<i <<n->get_name()<<" = updated"<<endl;
			(n->Child[i].parent) = n;
			update_by_bvh(frame,&(n->Child[i]));
		}
	}
	glm::mat4x4 m2 = n->get_translate_mat()*n->get_rotation_mat();
	if(n->get_name()=="pelvis"){
		n->global_tranform = m2;

	}
	else{
		n->global_tranform = n->parent->global_tranform * m2 ;
	}
	n->set_global_pos();

}
glm::mat4x4 Body_structure::make_rot_mat(glm::vec3 euler){
	glm::mat4x4 rot;
	//sequence = ZXY
	float Zangle=euler[0]/57.2957;
	float Xangle= euler[1]/57.2957;
	float Yangle=euler[2]/57.2957;
	rot = glm::rotate(rot,Zangle,glm::vec3(0,0,1));		//Z
	rot = glm::rotate(rot,Xangle,glm::vec3(1,0,0));	//X
	rot = glm::rotate(rot,Yangle,glm::vec3(0,1,0));	//Y
	return rot;
}
