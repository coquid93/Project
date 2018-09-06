#version 120

attribute vec3 position;
attribute vec3 normal;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;
uniform mat3 NormalMatrix;
// varying vec4 fragVert;
varying vec3 fragNormal;

varying vec4 world_vert;
void main(void) {
   world_vert = vec4(position,1);
   fragNormal = (view_mat* model_mat*vec4(normal,0)).xyz;
   // fragVert =view_mat* model_mat * vec4(position , 1);
   gl_Position = proj_mat * view_mat* model_mat * vec4(position , 1.0); // 바 뀔가능성 도있음..
}
