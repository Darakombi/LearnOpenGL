#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	fragPos = vec3(model * vec4(a_Pos, 1.0));
	normal = mat3(transpose(inverse(model))) * a_Normal;
	textureCoord = a_TextureCoord;

	gl_Position = proj * view * model * vec4(a_Pos, 1.0);
}

#shader fragment
#version 330 core

out vec4 finalColor;

in vec3 fragPos;
in vec3 normal;
in vec2 textureCoord;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float innerCutoff;
	float outerCutoff;
};

#define NR_POINT_LIGHTS 2

uniform Material material;

uniform vec3 u_ViewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 fragPos, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 fragPos, vec3 normal, vec3 viewDir);

void main()
{
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(u_ViewPos - fragPos);

	vec3 result = vec3(0.0);
	result += CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], fragPos, norm, viewDir);
	}
	result += CalcSpotLight(spotLight, fragPos, norm, viewDir);

	finalColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	vec3 reflectDir = reflect(lightDir, normal);

	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, textureCoord).rgb;

	//diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoord).rgb;

	//specular
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, textureCoord).rgb;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, textureCoord).rgb;

	//diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoord).rgb;

	//specular
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, textureCoord).rgb;

	//attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, textureCoord).rgb;

	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureCoord).rgb;

	//specular
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, textureCoord).rgb;

	//intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	//attenuation
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	ambient *= intensity * attenuation;	
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;

	return (ambient + diffuse + specular);
}