#include "../external/GL/glut.h"
#include "../include/Geometry.h"
#include <vector>
#include <string>
using namespace std;

class render2tex {
public:
        render2tex(){}
        ~render2tex(){}

        void init_FBO(string type , int tex_width,int tex_height , string s);
        void gen_texture(string type );
        void draw_debug_texture();
        void gen_shadow_map( GLfloat* light_pos);
        GLuint get_texID();

        GLuint texture_id;
        GLuint texture_rgb_id;
        GLuint fbo_id;
        string tex_type;
        float view_mat[16];
        float proj_mat[16];
        float texture_width;
        float texture_height;
        Geometry* light;
        int light_switch;
        string light_type;
};
