#version 410
layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 fragment_color;

uniform sampler2D back;
uniform sampler2D depth;

float blurclamp = 0.5;  // max blur amount
float bias = 2.0;	//aperture - bigger values for shallower depth of field
float focus = 0.990f;  // this value comes from ReadDepth script.
 
void main() 
{
	float aspectratio = 1080.0f / 720.0f;
	vec2 aspectcorrect = vec2(1.0,aspectratio);
	
	vec4 depth1   = texture2D(depth,UV );

	float factor = ( depth1.x - focus );
	 
	vec2 dofblur = vec2 (clamp( factor * bias, -blurclamp, blurclamp ));

	vec4 col = vec4(0.0);
	
	col += texture2D(back, UV);
	col += texture2D(back, UV + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur);
	col += texture2D(back, UV + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur);	
	col += texture2D(back, UV + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur);
	
	col += texture2D(back, UV + (vec2( 0.15,0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(back, UV + (vec2( -0.37,0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(back, UV + (vec2( 0.37,-0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(back, UV + (vec2( -0.15,-0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(back, UV + (vec2( -0.15,0.37 )*aspectcorrect) * dofblur*0.9);
	col += texture2D(back, UV + (vec2( 0.37,0.15 )*aspectcorrect) * dofblur*0.9);		
	col += texture2D(back, UV + (vec2( -0.37,-0.15 )*aspectcorrect) * dofblur*0.9);	
	col += texture2D(back, UV + (vec2( 0.15,-0.37 )*aspectcorrect) * dofblur*0.9);	
	
	col += texture2D(back, UV + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.7);
	col += texture2D(back, UV + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(back, UV + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(back, UV + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(back, UV + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.7);
	col += texture2D(back, UV + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(back, UV + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.7);	
	col += texture2D(back, UV + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.7);
			 
	col += texture2D(back, UV + (vec2( 0.29,0.29 )*aspectcorrect) * dofblur*0.4);
	col += texture2D(back, UV + (vec2( 0.4,0.0 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(back, UV + (vec2( 0.29,-0.29 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(back, UV + (vec2( 0.0,-0.4 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(back, UV + (vec2( -0.29,0.29 )*aspectcorrect) * dofblur*0.4);
	col += texture2D(back, UV + (vec2( -0.4,0.0 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(back, UV + (vec2( -0.29,-0.29 )*aspectcorrect) * dofblur*0.4);	
	col += texture2D(back, UV + (vec2( 0.0,0.4 )*aspectcorrect) * dofblur*0.4);	
			
	fragment_color = col/41.0;
	fragment_color.w = 1.0;
}