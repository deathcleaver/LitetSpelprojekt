#version 430
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout(location = 0) in vec4 color[];

layout(location = 0) out vec4 center;
layout(location = 1) out vec4 cornerPos;
layout(location = 2) out vec4 outColor;
layout(location = 3) out float size;

uniform mat4 V;
uniform mat4 P;

uniform vec3 camPos;

vec3 upFixed = vec3(0, 1, 0);

void main()
{
 
    mat4 VP = P*V;
    vec3 partPos = gl_in[0].gl_Position.xyz;
    float partSize = gl_in[0].gl_Position.w * 0.5;
    
    vec3 dist = (partPos - camPos);
    
    vec3 strafe = normalize(cross(dist, upFixed)) * partSize;
    vec3 up = normalize(cross(dist, strafe)) * partSize;
    
    outColor = color[0];
    center = VP * vec4(partPos, 1);
    
    size = partSize;
    
    // Top left
    cornerPos = gl_Position = VP * vec4(partPos - strafe + up, 1);
    EmitVertex();
    
    // bottom left
    cornerPos = gl_Position = VP * vec4(partPos - strafe - up, 1);
    EmitVertex();
    
    // top right
    cornerPos = gl_Position = VP * vec4(partPos + strafe + up, 1);
    EmitVertex();
    
    // bottom right
    cornerPos = gl_Position = VP * vec4(partPos + strafe - up, 1);
    EmitVertex();
    
    EndPrimitive();
    
}