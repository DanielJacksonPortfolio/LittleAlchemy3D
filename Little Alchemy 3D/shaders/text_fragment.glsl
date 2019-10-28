#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D textureAtlas;
uniform vec3 textColour;
uniform bool dropShadow;
uniform float textHeight;

const float width = 0.5;
const float minEdge = 0.03;
const float maxEdge = 0.2;

const float boarderWidth = 0.5;
const float boarderEdge = 0.3;

const vec3 outlineColour = vec3(0.0,0.0,0.0);

const vec2 outlineOffset = vec2(-0.004,-0.004);

void main()
{    
	float edgeFloat = 1.0 - smoothstep(15.0,75.0,textHeight);
	float edge = minEdge + (maxEdge - minEdge) * edgeFloat;
	
	if (dropShadow) {
		float dist = 1.0 - texture(textureAtlas, TexCoords).a;
		float alpha = 1.0 - smoothstep(width,width + edge,dist);
		
		float dist2 = 1.0 - texture(textureAtlas, TexCoords + outlineOffset).a;
		float outlineAlpha = 1.0 - smoothstep(boarderWidth,boarderWidth + boarderEdge,dist2);
		
		float overallAlpha = alpha + (1.0-alpha) * outlineAlpha;
		vec3 overallColour = mix(outlineColour, textColour, alpha / overallAlpha);
		
		FragColor = vec4(overallColour, overallAlpha);
	}
	else {
		float dist = 1.0 - texture(textureAtlas, TexCoords).a;
		float alpha = 1.0 - smoothstep(width,width + edge,dist);
		FragColor = vec4(textColour, alpha);
	}
}