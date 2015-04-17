#version 410
layout(location = 0) in vec2 UV;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 worldPos;
uniform sampler2D textureSample;

layout(location = 1) out vec4 diffuseOut;
layout(location = 2) out vec4 normalOut;
layout(location = 3) out vec4 worldOut;

void main () 
{
    diffuseOut = texture(textureSample,vec2(UV.s, UV.t));
    worldOut = vec4(worldPos, 1);
    normalOut = vec4(normal, 1);
}
