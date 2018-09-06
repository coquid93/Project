#include "../external/GL/glew.h"
#include "../external/GL/glext.h"
#include "../include/Geometry.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "../external/GL/freeglut.h"

#include <math.h>

#pragma warning(disable:4996)

using namespace std;
using namespace Eigen;

Geometry::Geometry()
{
	Vertices.clear();
	Normals.clear();
	indices.clear();
	hit = false;
}
Geometry::~Geometry()
{
	Vertices.clear();
	Normals.clear();
	TextureCoord.clear();
	indices.clear();
	type.clear();
	triangles.clear();
}
void Geometry::trans_geo(Vector3d v){
	orientation(3,0) += v(0);	orientation(3,1) += v(1);	orientation(3,2) += v(2);
	return;
}


void Geometry::loadObj(std::string filename)
{
//	fstream file;
//	string line;
//	file.open(filename.c_str(),fstream::in);
//	if(!file.is_open())
//	{
//		string str="Model File Not Found: ";
//		str+=filename;
//		return;
//	}
//	unsigned int n=0;
//	Vector3f v;
//	Vector2f vt;
//	Vector3f vn;
//
//	//face
//	Vec3i iv;
//	Vec3i ivt;
//	Vec3i ivn;
//	Vec3i quad_temp;
//	std::vector<int> temp_indices;
//	temp_indices.clear();
//	temp_indices.clear();
//	char buf[128];
//	while(!file.eof())
//	{
//		getline(file,line);
//		if(line.length()>2)
//		switch(line.at(0))
//		{
//			case 'm':
//				  sscanf(line.c_str(), "%s %s", buf, buf);
//			case 'v':
//					//vertex or tex coord
//					if(line.at(1)==' ')
//					{
//						//vertex
//						n=sscanf(line.c_str(),"v %f %f %f",&v.x,&v.y,&v.z);
//						if(n!=3)
//						{
//							v.x=0;
//							v.y=0;
//							v.z=0;
//							printf("failv!\n");
//						}
//						Vertices.push_back(v);
//						vert_Buf.push_back(v.x);
//						vert_Buf.push_back(v.y);
//						vert_Buf.push_back(v.z);
//					}
//					else if(line.at(1)=='n')
//					{
//						//normal
//						n=sscanf(line.c_str(),"vn %f %f %f",&vn.x,&vn.y,&vn.z);
//						if(n!=3)
//						{
//							vn.x=0;
//							vn.y=0;
//							vn.z=0;
//							printf("normal failn!\n");
//						}
//						Normals.push_back(vn);
//						norm_Buf.push_back(vn.x);
//						norm_Buf.push_back(vn.y);
//						norm_Buf.push_back(vn.z);
//					}
//					else if(line.at(1)=='t')
//					{
//						//texture coord
//						n=sscanf(line.c_str(),"vt %f %f",&vt.x,&vt.y);
//						if(n!=2)
//						{
//							vt.x=0;
//							vt.y=0;
//							printf("failt!\n");
//						}
//						TextureCoord.push_back(vt);
//					}
//			break;
//
//			case 'f':
//				quad_temp.Set(-1);
//				n=sscanf(line.c_str(),"f %d/%d/%d "
//									"%d/%d/%d "
//									"%d/%d/%d"
//									"%d/%d/%d",
//									&iv.x,&ivt.x,&ivn.x,
//									&iv.y,&ivt.y,&ivn.y,
//									&iv.z,&ivt.z,&ivn.z,
//									&quad_temp.x,&quad_temp.y,&quad_temp.z
//									);
//				if(n==9){
//					n=sscanf(line.c_str(),"f %d/%d/%d "
//									"%d/%d/%d "
//									"%d/%d/%d",
//									&iv.x,&ivt.x,&ivn.x,
//									&iv.y,&ivt.y,&ivn.y,
//									&iv.z,&ivt.z,&ivn.z
//									);
//					triangle temp;
//					temp.n[0]=ivn.x-1; temp.n[1]=ivn.y-1; temp.n[2]=ivn.z-1;
//					temp.t[0]=ivt.x-1; temp.t[1]=ivt.y-1; temp.t[2]=ivt.z-1;
//					temp.v[0]=iv.x-1; temp.v[1]=iv.y-1; temp.v[2]=iv.z-1;
//					triangles.push_back(temp);
//					indices.push_back(temp.v[0]);
//					indices.push_back(temp.v[1]);
//					indices.push_back(temp.v[2]);
//				}
//				else
//				{
//					n=sscanf(line.c_str(),"f %d/%d/%d "
//									"%d/%d/%d "
//									"%d/%d/%d"
//									"%d/%d/%d",
//									&iv.x,&ivt.x,&ivn.x,
//									&iv.y,&ivt.y,&ivn.y,
//									&iv.z,&ivt.z,&ivn.z,
//									&quad_temp.x,&quad_temp.y,&quad_temp.z
//									);
//					triangle temp;
//					temp.n[0]=ivn.x-1; temp.n[1]=ivn.y-1; temp.n[2]=ivn.z-1;
//					temp.t[0]=ivt.x-1; temp.t[1]=ivt.y-1; temp.t[2]=ivt.z-1;
//					temp.v[0]=iv.x-1; temp.v[1]=iv.y-1; temp.v[2]=iv.z-1;
//					triangles.push_back(temp);
//					indices.push_back(temp.v[0]);
//					indices.push_back(temp.v[1]);
//					indices.push_back(temp.v[2]);
//					triangle temp1;
//					temp1.n[0]=quad_temp.z-1; temp1.n[1]=ivn.x-1; temp1.n[2]=ivn.z-1;
//					temp1.t[0]=quad_temp.y-1; temp1.t[1]=ivt.x-1; temp1.t[2]=ivt.z-1;
//					temp1.v[0]=quad_temp.x-1; temp1.v[1]=iv.x-1; temp1.v[2]=iv.z-1;
//					triangles.push_back(temp1);
//					indices.push_back(temp1.v[0]);
//					indices.push_back(temp1.v[1]);
//					indices.push_back(temp1.v[2]);
//
//				}
//
//			break;
//
//			default:
//
//			break;
//		}
//	}
//	file.close();
//
//	printf("Vertex Normal num: %d %d\n",Vertices.size(),Normals.size());

}
void Geometry::set_cube(float r, float l, float h){

}
void Geometry::set_sphere(float r,double x,double y,double z){
	orientation.setIdentity();
	orientation.block<1,4>(3,0) = Vector4d(x,y,z,1.0f);
	const int Faces[] = {
	        2, 1, 0,
	        3, 2, 0,
	        4, 3, 0,
	        5, 4, 0,
	        1, 5, 0,
	        11, 6,  7,
	        11, 7,  8,
	        11, 8,  9,
	        11, 9,  10,
	        11, 10, 6,
	        1, 2, 6,
	        2, 3, 7,
	        3, 4, 8,
	        4, 5, 9,
	        5, 1, 10,
	        2,  7, 6,
	        3,  8, 7,
	        4,  9, 8,
	        5, 10, 9,
	        1, 6, 10 };

	const float Verts[] = {
		 0.000f,  0.000f,  1.000f,
		 0.894f,  0.000f,  0.447f,
		 0.276f,  0.851f,  0.447f,
		-0.724f,  0.526f,  0.447f,
		-0.724f, -0.526f,  0.447f,
		 0.276f, -0.851f,  0.447f,
		 0.724f,  0.526f, -0.447f,
		-0.276f,  0.851f, -0.447f,
		-0.894f,  0.000f, -0.447f,
		-0.276f, -0.851f, -0.447f,
		 0.724f, -0.526f, -0.447f,
		 0.000f,  0.000f, -1.000f };

	for (int i = 0; i < sizeof(Verts); i++) {
		vert_Buf.push_back(Verts[i]*r);
		norm_Buf.push_back(Verts[i]);
	}
	for (int i = 0; i < sizeof(Faces); i++) {
		indices.push_back(Faces[i]);
	}
	shader_type = "smoothing";
	radius = r;

}
void Geometry::setBuffer(){
	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	// Create the VBO for positions:
	glGenBuffers(1,&vboID);
	glBindBuffer(GL_ARRAY_BUFFER,vboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vert_Buf[0])*vert_Buf.size(),&vert_Buf[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);
	// Create the VBO for normals:
	glGenBuffers(1,&nboID);
	glBindBuffer(GL_ARRAY_BUFFER,nboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(norm_Buf[0])*norm_Buf.size(),&norm_Buf[0],GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(1);
	// Create the VBO for indices:
	glGenBuffers(1,&indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0])*indices.size(),&indices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void Geometry::Render()
{
	// Create the VAO:

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexID);
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	glDrawElements(GL_PATCHES,indices.size(),GL_UNSIGNED_INT,(void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


}
