#version 410
layout(location = 0) in vec2 UV;

out vec4 fragment_color;

uniform sampler2D depth;
uniform sampler2D back;

vec4 red = vec4(1, 0, 0, 1);
vec4 blue = vec4(0, 0, 1, 1);

void main () 
{
	//vec4 col = texture( back, vec2(UV));
	float d = texture( depth , vec2(UV)).x;
	
	if (d < 0.975) 
		fragment_color = vec4(0, 0.33, 0, 1); // 3 closest	
	else if (d < 0.985) 
		fragment_color = vec4(0, 0.66, 0, 1); // 2 closer
	else if (d < 0.989) 
		fragment_color = vec4(0, 1, 0, 1); // 1 close
	else if (d < 0.991) 
		fragment_color = vec4(1, 1, 1, 1); //player dist
	else if (d < 0.993)
		fragment_color = vec4(0, 0, 1, 1); // 1 far  
	else if (d < 0.995)
		fragment_color = vec4(0, 0, 0.66, 1); // 2 further
	else if (d < 0.998)
		fragment_color = vec4(0, 0, 0.33, 1); // 3 furthest
	else
		fragment_color = vec4(0, 0, 0, 1);

}