#version 120
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

varying vec4 fragVert;
varying vec3 fragNormal;
varying vec4 world_vert;

struct Light{
        mat4 view_mat;
        mat4 proj_mat;
        vec4 position;
        int turn;  //if turn on turn == 1; off == 0;
        sampler2D shadow_map;
};

uniform Light light[3];

void main () {
        mat4 half_mat;
        half_mat[0] = vec4(0.5 , 0 , 0 , 0);
        half_mat[1] = vec4(0 , 0.5 , 0 , 0);
        half_mat[2] = vec4(0 , 0 , 1 , 0);
        half_mat[3] = vec4(0.5 , 0.5 ,0,1);
        vec4 o_color = vec4(0,0,0,1);
        for(int i=0 ; i < 3 ; i++)
        {
//in world position
                if( light[i].turn == 1){
                        float visability = 1.0;
                        vec4 tex_coord = (light[i].proj_mat * light[i].view_mat * model_mat * world_vert);
                        // tex_coord = tex_coord / tex_coord[3];
                        tex_coord = tex_coord / tex_coord[3];
                        tex_coord = half_mat * tex_coord;
                        float depth_at_this = texture2D(light[i].shadow_map , vec2(tex_coord.xy)).r;
                        // gl_FragColor = vec4(tex_coord[2]);
                        // gl_FragColor = vec4(depth_at_this);
                        if( tex_coord[2] > depth_at_this+0.01)
                                visability = 0.1;
                        vec3 camera_pos =vec3(view_mat* vec4(0,0,0,1));
                        vec3 surfacePos = (fragVert).xyz;
                        vec3 surfaceToLight ;
                        if(light[i].position[3] > 0.5f){
                                surfaceToLight = normalize((view_mat * vec4(light[i].position.xyz,1)).xyz - surfacePos);
                        }
                        else {
                                surfaceToLight = normalize(-(view_mat * (light[i].position - vec4(0,0,0,1))).xyz);
                        }

                        vec3 surfaceToCamera = normalize(-surfacePos);

                        //attenuation
                        float distanceToLight = length(light[i].position.xyz - surfacePos);
                        // float attenuation = 1.0 / (1.0 + 1 * pow(distanceToLight, 1));
                        float attenuation = 1.0;

                        float diffuseCoefficient = max(0.0, dot(normalize(fragNormal), surfaceToLight));
                        vec3 diffuse = diffuseCoefficient * gl_FrontMaterial.diffuse.xyz * gl_LightSource[0].diffuse.xyz;

                        //specular
                        float specularCoefficient = 0.0;
                        if(diffuseCoefficient > 0.0)
                                specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normalize(fragNormal)))), 20);
                        vec3 specular = specularCoefficient *gl_FrontMaterial.specular.xyz * gl_LightSource[0].specular.xyz;
                        vec3 linearColor = visability*attenuation*(diffuse + specular);
                        o_color += vec4(linearColor,0);
                        // o_color += vec4((fragNormal),1);
                }
        }

        gl_FragColor = o_color;

}
