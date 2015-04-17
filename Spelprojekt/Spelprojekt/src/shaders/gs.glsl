#version 410
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
    gl_in[0].gl_Position *= modelMatrix;
    gl_in[1].gl_Position *= modelMatrix;
    gl_in[2].gl_Position *= modelMatrix;
    
    vec3 ab = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 ac = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    
    normal = normalize(cross( ab, ac));

    for( int i = 0; i < 3; i++ )
    {
        gl_Position =  P * V * (gl_in[i].gl_Position);
        UVCord = UV[i];
        worldPos = (gl_in[i].gl_Position).xyz;
        EmitVertex();
    }
    EndPrimitive();
}