#version 430
layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

layout(location = 0) in vec2 UV[];

uniform mat4 modelMatrix;
uniform mat4 V;
uniform mat4 P;

layout(location = 0) out vec2 UVCord;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec3 worldPos;

void main()
{
    vec4 pos0 = gl_in[0].gl_Position * modelMatrix;
    vec4 pos1 = gl_in[1].gl_Position * modelMatrix;
    vec4 pos2 = gl_in[2].gl_Position * modelMatrix;
    
    vec3 ac = (pos2 - pos0).xyz;
    vec3 ab = (pos1 - pos0).xyz;
    
    normal = normalize(cross( ab, ac));

    for( int i = 0; i < 3; i++ )
    {
        gl_Position = P * V * (gl_in[i].gl_Position * modelMatrix);
        UVCord = UV[i];
        worldPos = (gl_in[i].gl_Position * modelMatrix).xyz;
        EmitVertex();
    }
    EndPrimitive();
}