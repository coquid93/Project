#version 430 core

out vec4 FragColor;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;
in float gPrimitive;
uniform mat4 view_mat;
uniform vec3 light_pos;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;
uniform int hit;
float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
    vec3 N = normalize(gFacetNormal);
    vec3 L = normalize((view_mat*vec4(light_pos.xyz,0)).xyz);
    float df = (dot(N, L));
    vec3 color = normalize(normalize(AmbientMaterial) + df * normalize(DiffuseMaterial));

    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    
    if(hit == 0)color = amplify(d1, 300, -0.1) * amplify(d2, 300, -0.1) * color;

    FragColor = vec4(color, 1.0);
}
