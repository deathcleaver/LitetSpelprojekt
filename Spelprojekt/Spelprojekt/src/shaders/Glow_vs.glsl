#version 430
layout(location = 0) in vec4 lightPos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 other;

layout(location = 0) out vec4 outColor;

void main () 
{
    gl_Position = lightPos;
    outColor = color;
}