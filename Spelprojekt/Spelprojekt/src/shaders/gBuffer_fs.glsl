#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D textureSample;

layout(location = 1) out vec4 diffuseOut;
layout(location = 2) out vec4 textureOut;

void main () 
{
    diffuseOut = texture(textureSample,vec2(UV.s, UV.t));
    textureOut = vec4(UV, 0, 1);
}
