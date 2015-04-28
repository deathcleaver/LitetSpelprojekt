#version 430
layout(location = 0) in vec2 UV;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 worldPos;
uniform sampler2D textureSample;

layout(location = 1) out vec4 diffuseOut;
layout(location = 2) out vec4 normalOut;
layout(location = 3) out vec4 worldOut;

out vec4 fragment_color;

void main () 
{
	vec4 col = vec4(texture(textureSample,vec2(UV.s, UV.t)).rgb, 1);
    diffuseOut = col;
    worldOut = vec4(worldPos, 1);
    normalOut = vec4(normal, 1);
    
}
