#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D textureAtlas;
uniform vec3 textColour;

const float width = 0.5;
const float edge = 0.05;

const float boarderWidth = 0.5;
const float boarderEdge = 0.3;

const vec3 outlineColour = vec3(0.0,0.0,0.0);

const vec2 outlineOffset = vec2(-0.004,-0.004);

void main()
{    
	 float dist = 1.0 - texture(textureAtlas, TexCoords).a;
	 float alpha = 1.0 - smoothstep(width,width + edge,dist);
	
	 float dist2 = 1.0 - texture(textureAtlas, TexCoords + outlineOffset).a;
	 float outlineAlpha = 1.0 - smoothstep(boarderWidth,boarderWidth + boarderEdge,dist2);
	
	 float overallAlpha = alpha + (1.0-alpha) * outlineAlpha;
	 vec3 overallColour = mix(outlineColour, textColour, alpha / overallAlpha);
	
	 FragColor = vec4(overallColour, overallAlpha);
	// float value = texture(textureAtlas, TexCoords).a;
	// if (value < 0.5) discard;
	// else FragColor = vec4(textColour, 1.0f);
	
}