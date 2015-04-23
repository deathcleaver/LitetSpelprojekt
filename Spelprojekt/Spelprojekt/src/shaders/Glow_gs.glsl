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
    vec3 partPos = gl_in[0].gl_Position.xyz;
    float partSize = gl_in[0].gl_Position.w * 0.5f;
    
    vec3 dist = (partPos - camPos);

    float culldist = length(partPos.x - camPos.x);
	if(culldist < (15 + partSize * 0.2))
	{
		float culldist = length(partPos.y - camPos.y);
		if(culldist < (8 + partSize * 0.2))
		{
			vec3 strafe = normalize(cross(dist, upFixed));
			vec3 up = normalize(cross(dist, strafe));
			
			mat4 PV = P * V;
			
			outColor = color[0];
			center = PV * vec4(partPos, 1);
			
			size = partSize;
			
			// Top left
			cornerPos = gl_Position = PV * vec4(partPos - (strafe * partSize) + (up * partSize), 1);
			EmitVertex();
			
			// bottom left
			cornerPos = gl_Position = PV * vec4(partPos - (strafe * partSize) - (up * partSize), 1);
			EmitVertex();
			
			// top right
			cornerPos = gl_Position = PV * vec4(partPos + (strafe * partSize) + (up * partSize), 1);
			EmitVertex();
			
			// bottom right
			cornerPos = gl_Position = PV * vec4(partPos + (strafe * partSize) - (up * partSize), 1);
			EmitVertex();
			
			EndPrimitive();
		}
	}
}