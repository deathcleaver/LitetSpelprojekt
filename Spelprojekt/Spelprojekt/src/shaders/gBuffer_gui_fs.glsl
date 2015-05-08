#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D diffuse;

out vec4 fragment_color;

void main () 
{
    vec4 diffuseColor = texture(diffuse,vec2(UV.s, 1 - UV.t));
    if(diffuseColor.x > 0.9f && diffuseColor.y < 0.1f && diffuseColor.z > 0.9f) //pink
		diffuseColor.w = 0;
    fragment_color = diffuseColor;
}