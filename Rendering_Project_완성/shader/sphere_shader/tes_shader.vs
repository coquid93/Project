#version 300 
layout(location = 0 ) in vec4 Position;
layout(location = 1 ) in vec3 normal;

out vec3 vPosition;

void main()
{
    vPosition = Position.xyz;
}
