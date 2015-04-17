#version 410
layout(location = 0) in vec2 UV;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D world;

out vec4 fragment_color;

vec3 lightPos = vec3(0, 0, 4);
float lDist = 100;

vec3 lightColor = vec3(1, 1, 1);

void main () 
{
    
    vec4 worldPos = texture(world,vec2(UV.s, UV.t));
    vec4 n = texture(normal,vec2(UV.s, UV.t));
    vec4 diffuseColor = texture(diffuse,vec2(UV.s, UV.t));
    
    float dist = distance(worldPos.xyz, lightPos.xyz);
    
    vec4 letThereBeLight;
    
    if(dist < lDist)
    {
        float d = lDist;
        float attenuation;
        if(dist != 0)
            attenuation = 1- clamp((pow(dist,2) / d), 0, 1);
            attenuation = max(attenuation, 0);
        
        vec3 s = normalize(vec3(lightPos.xyz - worldPos.xyz));

        vec3 r = reflect(-s, n.xyz);
        
        letThereBeLight = vec4(lightColor.xyz * attenuation * max(dot(n.xyz, s), 0), 1.0);
    }
    
    fragment_color = diffuseColor * letThereBeLight;
    
}