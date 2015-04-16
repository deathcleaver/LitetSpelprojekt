#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D world;

out vec4 fragment_color;

void main () 
{
    
    
    if(UV.s < 0.5 && UV.t < 0.5)
    {
        fragment_color = texture(world,vec2(UV.s, UV.t));
    }
    else if(UV.s > 0.5 && UV.t < 0.5)
    {
        fragment_color = texture(normal,vec2(UV.s, UV.t));
    }
    else
    {
        fragment_color = texture(diffuse,vec2(UV.s, UV.t));
    }
}