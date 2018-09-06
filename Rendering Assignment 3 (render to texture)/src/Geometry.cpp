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
    scale_factor = 1.0f;
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


void Geometry::loadObj(std::string filename, float x, float y, float z )
{
    orientation.setIdentity();
    orientation.block<1,4>(3,0) = Vector4d(x,y,z,1.0f);


	fstream file;
	string line;
	file.open(filename.c_str(),fstream::in);
	if(!file.is_open())
	{
		string str="Model File Not Found: ";
		str+=filename;
        std::cout<<str<<std::endl;
		return;
	}
	unsigned int n=0;
	Vector3f v;
	Vector2f vt;
	Vector3f vn;

	//face
    Vector3i iv;
    Vector3i ivt;
    Vector3i ivn;
    Vector3i quad_temp;
	std::vector<int> temp_indices;
	temp_indices.clear();
	char buf[128];
	while(!file.eof())
	{
		getline(file,line);
		if(line.length()>2)
		switch(line.at(0))
		{
			case 'm':
				  sscanf(line.c_str(), "%s %s", buf, buf);
			case 'v':
					//vertex or tex coord
					if(line.at(1)==' ')
					{
						//vertex
						n=sscanf(line.c_str(),"v %f %f %f",&v(0),&v(1),&v(2));
						if(n!=3)
						{
							v(0)=0;
							v(1)=0;
							v(2)=0;
							printf("failv!\n");
						}
						Vertices.push_back(v);
						vert_Buf.push_back(v(0));
						vert_Buf.push_back(v(1));
						vert_Buf.push_back(v(2));
					}
					else if(line.at(1)=='n')
					{
						//normal
						n=sscanf(line.c_str(),"vn %f %f %f",&vn(0),&vn(1),&vn(2));
						if(n!=3)
						{
							vn(0)=0;
							vn(1)=0;
							vn(2)=0;
							printf("normal failn!\n");
						}
						Normals.push_back(vn);
						norm_Buf.push_back(vn(0));
						norm_Buf.push_back(vn(1));
						norm_Buf.push_back(vn(2));
					}
					break;

			case 'f':
                    quad_temp = Vector3i(-1,-1,-1);
                    n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d"
									"%d/%d/%d",
									&iv(0),&ivt(0),&ivn(0),
									&iv(1),&ivt(1),&ivn(1),
									&iv(2),&ivt(2),&ivn(2),
									&quad_temp(0),&quad_temp(1),&quad_temp(2)
									);
				if(n==9){
					n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d",
									&iv(0),&ivt(0),&ivn(0),
									&iv(1),&ivt(1),&ivn(1),
									&iv(2),&ivt(2),&ivn(2)
									);
					triangle temp;
					temp.n[0]=int(ivn(0)-1); temp.n[1]=int(ivn(1)-1); temp.n[2]=int(ivn(2)-1);
					temp.t[0]=int(ivt(0)-1); temp.t[1]=int(ivt(1)-1); temp.t[2]=int(ivt(2)-1);
					temp.v[0]=int(iv(0)-1); temp.v[1]=int(iv(1)-1); temp.v[2]=int(iv(2)-1);
					triangles.push_back(temp);
					indices.push_back(temp.v[0]);
					indices.push_back(temp.v[1]);
					indices.push_back(temp.v[2]);
				}
				else
				{
					n=sscanf(line.c_str(),"f %d/%d/%d "
									"%d/%d/%d "
									"%d/%d/%d"
									"%d/%d/%d",
									&iv(0),&ivt(0),&ivn(0),
									&iv(1),&ivt(1),&ivn(1),
                                    &iv(2),&ivt(2),&ivn(2),
									&quad_temp(0),&quad_temp(1),&quad_temp(2)
									);
					triangle temp;
					temp.n[0]=ivn(0)-1; temp.n[1]=ivn(1)-1; temp.n[2]=ivn(2)-1;
					temp.t[0]=ivt(0)-1; temp.t[1]=ivt(1)-1; temp.t[2]=ivt(2)-1;
					temp.v[0]=iv(0)-1; temp.v[1]=iv(1)-1; temp.v[2]=iv(2)-1;
					triangles.push_back(temp);
					indices.push_back(temp.v[0]);
					indices.push_back(temp.v[1]);
					indices.push_back(temp.v[2]);
					triangle temp1;
					temp1.n[0]=quad_temp(2)-1; temp1.n[1]=ivn(0)-1; temp1.n[2]=ivn(2)-1;
					temp1.t[0]=quad_temp(1)-1; temp1.t[1]=ivt(0)-1; temp1.t[2]=ivt(2)-1;
					temp1.v[0]=quad_temp(0)-1; temp1.v[1]=iv(0)-1; temp1.v[2]=iv(2)-1;
					triangles.push_back(temp1);
					indices.push_back(temp1.v[0]);
					indices.push_back(temp1.v[1]);
					indices.push_back(temp1.v[2]);

				}

			break;

			default:

			break;
		}
	}
	file.close();
    std::cout<<"load complete"<<std::endl;
    printf("Vertex Normal num: %d %d\n",int(Vertices.size()),int(Normals.size()));
    if ( Vertices.size() < 5){
            for(int i =0; i < Vertices.size(); i++){
                    std::cout<<Vertices[i]<<std::endl;
            }
    }
    radius = 10;
}
void Geometry::set_plain(float size){
	orientation.setIdentity();
	orientation.block<1,4>(3,0) = Vector4d(0,0,0,1.0f);
	const int Faces[] = {
	        2, 1, 0,
	        3, 2, 0
	        };

	const float Verts[] = {
		 1.000f,  0.000f,  1.000f,
		-1.000f,  0.000f,  1.000f,
		-1.000f,  0.000f, -1.000f,
		 1.000f,  0.000f, -1.000f
		 };
    const float Norms[] = {
            0 , 1.0f , 0.0f ,
            0 , 1.0f , 0.0f ,
            0 , 1.0f , 0.0f ,
            0 , 1.0f , 0.0f
    };
	for (int i = 0; i < sizeof(Verts); i++) {
		vert_Buf.push_back(Verts[i]*size);
		norm_Buf.push_back(Norms[i]);
	}
	for (int i = 0; i < sizeof(Faces); i++) {
		indices.push_back(Faces[i]);
	}




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
void Geometry::setBuffer_shadow(GLuint prog){
	glGenVertexArrays(1,&svaoID);
	glBindVertexArray(svaoID);
	// Create the VBO for positions:
	glGenBuffers(1,&svboID);
	glBindBuffer(GL_ARRAY_BUFFER,svboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vert_Buf[0])*vert_Buf.size(),&vert_Buf[0],GL_STATIC_DRAW);
    GLuint loc_pos = glGetAttribLocation(prog,"position");
	glEnableVertexAttribArray(loc_pos);
	glVertexAttribPointer(loc_pos,3,GL_FLOAT,GL_FALSE,0,NULL);
	// Create the VBO for normals:

	glGenBuffers(1,&snboID);
	glBindBuffer(GL_ARRAY_BUFFER,snboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(norm_Buf[0])*norm_Buf.size(),&norm_Buf[0],GL_STATIC_DRAW);
    GLuint loc_norm = glGetAttribLocation(prog,"normal");
	glEnableVertexAttribArray(loc_norm);
	glVertexAttribPointer(loc_norm,3,GL_FLOAT,GL_FALSE,0,NULL);



	glGenBuffers(1,&sindexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sindexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0])*indices.size(),&indices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // gl_Vertex	0
    //         // gl_Normal	2
    //         // gl_Color	3
    //         gl_SecondaryColor	4
    //         gl_FogCoord	5
    //         gl_MultiTexCoord0	8
    //         gl_MultiTexCoord1	9
    //         gl_MultiTexCoord2	10
    //         gl_MultiTexCoord3	11
    //         gl_MultiTexCoord4	12
    //         gl_MultiTexCoord5	13
    //         gl_MultiTexCoord6	14
    //         gl_MultiTexCoord7	15
}
void Geometry::setBuffer(GLuint prog){

	glGenVertexArrays(1,&vaoID);
	glBindVertexArray(vaoID);
	// Create the VBO for positions:
	glGenBuffers(1,&vboID);
	glBindBuffer(GL_ARRAY_BUFFER,vboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vert_Buf[0])*vert_Buf.size(),&vert_Buf[0],GL_STATIC_DRAW);
    GLuint loc_pos = glGetAttribLocation(prog,"position");
	glEnableVertexAttribArray(loc_pos);
	glVertexAttribPointer(loc_pos,3,GL_FLOAT,GL_FALSE,0,NULL);
	// Create the VBO for normals:

	glGenBuffers(1,&nboID);
	glBindBuffer(GL_ARRAY_BUFFER,nboID);
	glBufferData(GL_ARRAY_BUFFER,sizeof(norm_Buf[0])*norm_Buf.size(),&norm_Buf[0],GL_STATIC_DRAW);
    GLuint loc_norm = glGetAttribLocation(prog,"normal");
	glEnableVertexAttribArray(loc_norm);
	glVertexAttribPointer(loc_norm,3,GL_FLOAT,GL_FALSE,0,NULL);



	glGenBuffers(1,&indexID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0])*indices.size(),&indices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // gl_Vertex	0
    //         // gl_Normal	2
    //         // gl_Color	3
    //         gl_SecondaryColor	4
    //         gl_FogCoord	5
    //         gl_MultiTexCoord0	8
    //         gl_MultiTexCoord1	9
    //         gl_MultiTexCoord2	10
    //         gl_MultiTexCoord3	11
    //         gl_MultiTexCoord4	12
    //         gl_MultiTexCoord5	13
    //         gl_MultiTexCoord6	14
    //         gl_MultiTexCoord7	15
}
void Geometry::Render()
{

        glBindVertexArray(vaoID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexID);
//	glPatchParameteri(GL_PATCH_VERTICES, 3);
        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,(void*)0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


}

void Geometry::Render_shadow()
{

	glBindVertexArray(svaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sindexID);
//	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,(void*)0);
	glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


}
