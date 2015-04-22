#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out vec2 UVCord;

in int gl_InstanceID;

uniform mat4 modelMatrix;
uniform mat4 VP;

uniform int useBuffer;

layout(std430, binding = 1) buffer MatriceBuffer
{
    mat4 matrice[];
};

void main () 
{
    UVCord = UV;
    
    if(useBuffer == 1)
    {
        gl_Position =  vec4(vertex_position, 1.0f) * modelMatrix;
    }
    else
    {
        gl_Position =  vec4(vertex_position, 1.0f) * matrice[gl_InstanceID];
    }
}