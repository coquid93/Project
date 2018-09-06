#version 120

// deferred rendering vertex shader
// G-Buffer!!!!
attribute vec3 position;

// uniform mat4 proj_mat;
// uniform mat4 view_mat;
// uniform mat4 model_mat;

varying vec2 texture_coord;

void main(void) {
        texture_coord = position.xy*0.5 + 0.5;
        gl_Position = vec4(position , 1.0); // 바 뀔가능성 도있음..
}
