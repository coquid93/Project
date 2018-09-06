#include "../external/GL/glut.h"
#include "../include/Geometry.h"
#include "../include/glslShader.h"
#include <vector>
#include <string>
using namespace std;

class render2tex {
public:
        render2tex(){}
        ~render2tex(){}

        void init_FBO(string type , int tex_width,int tex_height , string s);
        void gen_texture(string type );
        void draw_debug_texture(string texture_type);
        void gen_shadow_map( GLfloat* light_pos);
        void gen_ssao_map( glslShader * shadow_shader ,  std::vector<Geometry* >geo_item );
        GLuint get_texID();

        GLuint texture_norm_id;
        GLuint texture_rgb_id;
        GLuint texture_depth_id;
        GLuint texture_pos_id;
        GLuint texture_amb_id;
        GLuint texture_diff_id;
        GLuint texture_spec_id;
        GLuint texture_ori_id;

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
