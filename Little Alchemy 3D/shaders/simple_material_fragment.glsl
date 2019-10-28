#version 330 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	
	vec3 diffuse_colour;
	vec3 specular_colour;
	float shininess;
	float reflectivity;
};

struct DirLight
{
    vec3 direction;
	vec3 diffuse;
	vec3 specular;
    vec3 ambient;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float visibility;

in vec4 FragPosLightSpace;

out vec4 colour;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform samplerCube skybox;

uniform sampler2D shadowMap;
uniform vec3 lightPos;

uniform bool projectCubemapTexture;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir, vec3 textureCol);
vec3 CalcSpotLight(SpotLight light, vec3 Normal, vec3 fragPos, vec3 viewDir, vec3 textureCol);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{	
	vec3 viewDir = normalize( viewPos - FragPos );
	
	//vec3 norm = CalcNormal();
	vec3 norm = normalize(Normal);
	vec3 textureCol = vec3(1.0,1.0,1.0);
	if (projectCubemapTexture) {
		textureCol = texture(skybox,normalize(FragPos - vec3(0.0,15.0,0.0))).xyz;
	}
    vec3 result = CalcDirLight( dirLight, norm, viewDir, textureCol);
	// for(int i = 0; i < NR_POINT_LIGHTS; i++)
        //result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir, textureCol);    
	// * vec3(texture(material.texture_specular1, TexCoords))
	vec3 TextureCoords3d = reflect(-viewDir,norm);
	result = mix(texture(skybox,vec3(-TextureCoords3d.x,TextureCoords3d.y,TextureCoords3d.z)).xyz,result,1.0 - (material.reflectivity * (0.25 + 0.75 * clamp(1.0f - 0.5f * abs(dot(norm,-viewDir)),0.0f,1.0f))));//
	
    colour = vec4(result,visibility);
}


vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir, vec3 textureCol)
{
    vec3 lightDir = normalize( -light.direction );
	
    vec3 ambient = light.ambient  * material.diffuse_colour * textureCol;
    vec3 diffuse = light.diffuse * max( dot( normal, lightDir ), 0.0 )  * material.diffuse_colour * textureCol;

	vec3 specular = light.specular * pow( max( dot( viewDir, reflect( -lightDir, normal ) ), 0.0 ), material.shininess ) * material.specular_colour;
	
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);       
	
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * material.diffuse_colour * material.diffuse_colour;
}


vec3 CalcSpotLight(SpotLight light, vec3 Normal, vec3 fragPos, vec3 viewDir, vec3 textureCol) {
	vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	float dist= length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	vec3 ambient =light.ambient * intensity * attenuation * material.diffuse_colour * textureCol;
    vec3 diffuse = light.diffuse * max(dot(Normal, lightDir), 0.0) * intensity * attenuation * material.diffuse_colour * textureCol;
    vec3 specular = light.specular * pow(max(dot(viewDir, reflect(-lightDir, Normal)), 0.0), material.shininess) * material.specular_colour * attenuation;
    
	return (ambient + diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	//float bias = 0.0005*tan(acos(dot(normal,lightDir))); 
	float bias = 0.00005;
	bias = max(0.001 * (1.0 - dot(normal, lightDir)), 0.00005);  
	bias = gl_FrontFacing ? bias : 0.0;
	//bias = clamp(bias, 0,0.005);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;
	float distToFrag = length(viewPos - FragPos);
	if (distToFrag > 40)
		shadow *= clamp(2-(distToFrag-40)*0.1,0.0,1.0);
    return shadow;
}  