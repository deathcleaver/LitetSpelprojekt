#version 430
layout(location = 0) in vec2 UV;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D world;

out vec4 fragment_color;

struct Light
{
    vec4 pos;
    vec4 color;
    float distance;
    float intensity;
    float pad;
    float pad2;
};

uniform vec3 playerPos;
uniform uint nrLights;
float lDist = 100;

layout(std140, binding = 0) uniform lightBlock
{
    Light light[10];
};

void main () 
{
    
    vec4 worldPos = texture(world,vec2(UV.s, UV.t));
    vec4 n = texture(normal,vec2(UV.s, UV.t));
    vec4 diffuseColor = texture(diffuse,vec2(UV.s, UV.t));
    
    vec4 letThereBeLight = vec4(0);
    
    for(int i = 0; i < nrLights; i++)
    {
        Light l = light[i]; 
        float dist = distance(worldPos.xyz, l.pos.xyz);
        
        float d = l.distance;
        
        if(dist < d)
        {
            float attenuation = 1.0;
            if(dist != 0)
                attenuation = 1 - clamp((pow(dist,1.5) / d), 0, 1);
                attenuation = max(attenuation, 0);
            
            vec3 s = normalize(vec3(l.pos.xyz - worldPos.xyz));

            vec3 r = reflect(s, n.xyz);
            
            letThereBeLight += vec4(l.color.rgb * attenuation * max(dot(n.xyz, s), 0), 1.0);
        }
    }
    
    Light l;
    l.pos = vec4(playerPos.xyz, 0.0);
    l.color = vec4(1, 1, 1, 1);
    l.distance = 25.f;
    l.intensity = 1.0f;
    
    float dist = distance(worldPos.xyz, l.pos.xyz);
    
    float d = l.distance;
    
    if(dist < d)
    {
        float attenuation = 1.0;
        if(dist != 0)
            attenuation = 1 - clamp((pow(dist,1.5) / d), 0, 1);
            attenuation = max(attenuation, 0);
        
        vec3 s = normalize(vec3(l.pos.xyz - worldPos.xyz));

        vec3 r = reflect(s, n.xyz);
        
        letThereBeLight += vec4(l.color.rgb * attenuation * max(dot(n.xyz, s), 0), 1.0);
    
    }
    fragment_color = diffuseColor * letThereBeLight;
    
}