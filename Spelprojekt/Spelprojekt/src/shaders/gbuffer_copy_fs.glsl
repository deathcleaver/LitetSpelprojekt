#version 430
layout(location = 0) in vec2 UV;

uniform sampler2D diffuseIn;
uniform sampler2D normalIn;
uniform sampler2D worldIn;

layout(location = 1) out vec4 diffuseOut;
layout(location = 2) out vec4 normalOut;
layout(location = 3) out vec4 worldOut;

out vec4 fragment_color;

void main () 
{
	vec4 col = vec4(texture(diffuseIn, vec2(1.0f - UV.s, UV.t)).rgb, 1);
    vec4 norm = vec4(texture(normalIn, vec2(1.0f - UV.s, UV.t)).rgb, 1);
    vec4 world = vec4(texture(worldIn, vec2(1.0f - UV.s, UV.t)).rgb, 1);
    diffuseOut = col;
    worldOut = world;
    normalOut = norm;
    
}
