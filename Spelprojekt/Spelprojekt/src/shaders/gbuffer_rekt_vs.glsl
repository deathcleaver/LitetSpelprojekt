#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out vec2 UVCord;

uniform mat4 modelMatrix;
uniform mat4 P;
uniform mat4 V;

void main () 
{
    UVCord = UV;
    gl_Position =  P * V * (vec4(vertex_position, 1) * modelMatrix);
}