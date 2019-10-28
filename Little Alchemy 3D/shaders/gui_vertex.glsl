#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform vec2 scale;
uniform vec2 transform;

void main()
{
	vec2 alteredPosition = position * scale;
	alteredPosition += transform;
    gl_Position = vec4(alteredPosition, 0.0, 1.0);
	TexCoords = texCoords;
} 