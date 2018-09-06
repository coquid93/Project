#include "../external/GL/glew.h"
#include "../include/render2texture.h"
#include <iostream>




void render2tex::init_FBO(string type, int tex_width, int tex_height , string light_type_input){
            std::cout << "Initializing FBO" << "\n";
            glGenTextures(1,&texture_id);
            glBindTexture(GL_TEXTURE_2D,texture_id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,tex_width,tex_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);  //실질적으로 texture가 있는것은 아니므로 NULL 바인딩.
            glBindTexture(GL_TEXTURE_2D, 0);

            std::cout << "Initializing FBO complete" << "\n";
            glGenTextures(1,&texture_rgb_id);
            glBindTexture(GL_TEXTURE_2D,texture_rgb_id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,tex_width,tex_height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            glGenFramebuffersEXT(1,&fbo_id);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fbo_id);
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,texture_id,0);
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,texture_rgb_id,0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            texture_width = tex_width;
            texture_height = tex_height;
            light_switch = 1; // initially turn on;
            light_type = light_type_input;
}
void render2tex::gen_texture(string type){
    //now we generate texture
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fbo_id);  // fbo에다가 그려준다
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,texture_width,texture_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, float(texture_width)/float(texture_height), 1, 100);
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	glTranslatef(0, 0, -6);
	glColor3f(1,1,0);
	glutSolidTeapot(0.5f);
    glPopMatrix();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);  // 언바인딩 해준다.

}

void render2tex::draw_debug_texture(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,texture_rgb_id); //fbo에 그려진 텍스쳐를 쓸거다.
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-0.5);
    glTexCoord2f(1,0); glVertex3f(-0.5,-1,-0.5);
    glTexCoord2f(1,1); glVertex3f(-0.5,-0.5,-0.5);
    glTexCoord2f(0,1); glVertex3f(-1,-0.5,-0.5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,0);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void render2tex::gen_shadow_map( GLfloat* light_pos ){
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0,0,texture_width,texture_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if(light_type == "point")
        {
                gluPerspective(110, float(texture_width)/float(texture_height), 20, 500);
        }
        else
        {
                glOrtho(-200, 200, -200, 200, -100,300);
        }
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(light_pos[0], light_pos[1], light_pos[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glGetFloatv(GL_PROJECTION_MATRIX, proj_mat);
		glGetFloatv(GL_MODELVIEW_MATRIX, view_mat);

}
GLuint render2tex::get_texID(){
        return texture_id;
}





















