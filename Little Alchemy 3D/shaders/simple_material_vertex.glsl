#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out float visibility;

out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

const float density = 0.05;
const float gradient = 2.0;

void main()
{
    gl_Position = projection * view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(mat3(model)))) * normal;
    TexCoords = texCoords;
	
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	
	vec4 positonRelativeToCam = view * model * vec4(position, 1.0f);
	visibility = 1.0f - smoothstep(20.0,30.0,length(vec3(FragPos.x,FragPos.y,FragPos.z)-vec3(0.0,0.0,0.0)));
	//float distance = length(positonRelativeToCam);
	//visibility = exp(-pow((distance*density),gradient));
	//visibility = clamp(visibility,1.0,1.0);
}