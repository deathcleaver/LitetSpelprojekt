#version 430

layout(location = 0) in vec4 center;
layout(location = 1) in vec4 fragmentPos;
layout(location = 2) in vec4 color;
layout(location = 3) in float maxDist;

uniform float fade;

layout(location = 1) out vec4 glowOut;
out vec4 fragment_color;

void main () 
{
    float dist = distance(fragmentPos.xyz, center.xyz) / maxDist;
    
    dist *= 2;
    
    float power = pow(1 - min(dist, 1), 4);
    power = clamp(power, 0, 1);

    glowOut = vec4((color.rgb * color.w), min(power * fade, 1));
    
    fragment_color = glowOut;
    
}
