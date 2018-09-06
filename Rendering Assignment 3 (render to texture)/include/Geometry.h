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
    Geometry(GLuint prog){program_id = prog;}
	void trans_geo(Eigen::Vector3d v);
    void get_programID(GLuint prog){program_id = prog;}
    void loadObj(std::string filename , float x, float y , float z);
	void set_plain(float size);
	void set_sphere(float r,double x,double y,double z);
    GLfloat* get_pos()
    {
        GLfloat* r = new GLfloat[4];
        r[0] = orientation(3,0); r[1] = orientation(3,1); r[2] = orientation(3,2); r[3] = 1;
        return r;
    }

	void setBuffer(GLuint prog);
        void setBuffer_shadow(GLuint prog);
	void Render();
    void Render_shadow();

public:
	std::vector<Eigen::Vector3f > Vertices;
	std::vector<Eigen::Vector3f > Normals;
	std::vector<Eigen::Vector2f > TextureCoord;
	std::vector<float> vert_Buf;
	std::vector<float> norm_Buf;
	std::vector<int> indices;
	std::vector<int> type;
	std::vector<triangle> triangles;

        GLuint vaoID;
        GLuint vboID;
        GLuint nboID;
        GLuint indexID;
        GLuint svaoID;
        GLuint svboID;
        GLuint snboID;
        GLuint sindexID;


//case : smoothing , normal
	std::string shader_type;
	bool hit;
	Eigen::Matrix4d orientation;
	double radius;
    GLuint program_id;
        GLuint shadow_prog_id;
        float scale_factor;
}; //end Geometry class
