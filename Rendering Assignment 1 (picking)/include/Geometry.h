#pragma once
#include <vector>
#include "../external/GL/freeglut.h"
#include "../external/Eigen/Eigen"
#include "../external/glm/glm.hpp"
typedef struct _triangle{
	int v[3];
	int n[3];
	int t[3];
} triangle;

class Geometry {
public:
	Geometry();
	~Geometry();
	void trans_geo(Eigen::Vector3d v);

	void loadObj(std::string filename);
	void set_cube(float r,float l, float h);
	void set_sphere(float r,double x,double y,double z);

	void setBuffer();
	void Render();

public:
	std::vector<Eigen::Vector3f> Vertices;
	std::vector<Eigen::Vector3f> Normals;
	std::vector<Eigen::Vector2f> TextureCoord;
	std::vector<float> vert_Buf;
	std::vector<float> norm_Buf;
	std::vector<int> indices;
	std::vector<int> type;
	std::vector<triangle> triangles;

	GLuint vaoID;
	GLuint vboID;
	GLuint nboID;
	GLuint indexID;
	//case : smoothing , normal
	std::string shader_type;
	bool hit;
	Eigen::Matrix4d orientation;
	double radius;
}; //end Geometry class
