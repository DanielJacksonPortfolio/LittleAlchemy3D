#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform bool useTexture;
uniform sampler2D GUITexture;
uniform vec3 colour;

void main()
{    
	if (useTexture) {
		FragColor = texture(GUITexture, TexCoords) * vec4(colour,1.0);
	}
	else {
		FragColor = vec4(colour,1.0);
	}
}