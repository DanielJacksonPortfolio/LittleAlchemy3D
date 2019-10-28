#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
//out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position.x,position.y, 0.0, 1.0);
	//FragPos = vec3(model * vec4(position, 0.0, 1.0));
	//gl_Position = projection * vec4(position, 0.0, 1.0);
	TexCoords = texCoords;
} 