#version 120
//render to texture shader ( frag )
uniform mat4 proj_mat;
uniform mat4 model_mat;
varying vec4 fragVert;
varying vec3 fragNormal;
varying vec4 ori_vert;
void main () {
        vec4 last_pos = proj_mat * fragVert;
        last_pos = last_pos / last_pos[3];
        //gl_FragCoord
        gl_FragData[0] = vec4(last_pos[2],last_pos[2],last_pos[2],1); // z data
        gl_FragData[1] = vec4(normalize(fragNormal).xyz *0.5 +0.5 , 0); // normal data
        gl_FragData[2] = fragVert;
        gl_FragData[3] = vec4(gl_FrontMaterial.ambient.xyzw) ;  // color data
        gl_FragData[4] = vec4(gl_FrontMaterial.diffuse.xyzw);  // color data
        gl_FragData[5] = vec4(gl_FrontMaterial.specular.xyzw);  // color data
        gl_FragData[6] = model_mat * ori_vert;

}
