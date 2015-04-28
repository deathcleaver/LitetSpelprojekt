#version 410
layout(location = 0) in vec2 UV;

out vec4 fragment_color;

void main () 
{
    vec4 diffuseColor = vec4(1,0,1, 0.4f);
    fragment_color = diffuseColor;
}