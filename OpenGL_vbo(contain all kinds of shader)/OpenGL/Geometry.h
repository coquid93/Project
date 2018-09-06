#pragma once
#include <vector>
#include "gl/freeglut.h"
#include "Math/Vec3.h"
#include "Math/Vec2.h"


typedef struct _triangle{
	int v[3];
	int n[3];
	int t[3];
} triangle;

class Geometry {
public:
	Geometry();
	~Geometry();
	void loadObj(std::string filename);
	void loadBMP(std::string filename);

	void Render();

public:
	std::vector<respace::Vec3f> Vertices;
	std::vector<respace::Vec3f> Normals;
	std::vector<respace::Vec2f> TextureCoord;
	std::vector<float> vert_Buf;
	std::vector<float> norm_Buf;
	std::vector<int> indices;
	std::vector<int> type;
	std::vector<triangle> triangles;

	GLuint vaoID;
	GLuint vboID;
	GLuint nboID;
	GLuint indexID;
	GLuint textID;

}; //end Geometry class
