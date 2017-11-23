#version 410 core
uniform vec4 uColor;			// used for input from program (CPU)

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLight;
in vec3 toCamera;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec4 lightConstants;

float ambientConstant;
float diffuseConstant;
float specularConstant;
float shininess;

out vec4 fragColor;

void main()
{
	ambientConstant = lightConstants.x;
	diffuseConstant = lightConstants.y;
	specularConstant = lightConstants.z;
	shininess = lightConstants.w;

	//AMBIENT LIGHTING
	vec3 ambient = ambientConstant * lightColor;

	// DIFFUSE LIGHTING
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitToLight = normalize(toLight);
	float diffBrightness = max( dot(unitNormal, unitToLight), 0);
	vec3 diffuse = diffBrightness * diffuseConstant * lightColor;

	//SPECULAR LIGHTING
	vec3 unitToCamera = normalize(toCamera);
	vec3 reflectedLight = 2 * dot(unitToLight, unitNormal) * unitNormal - unitToLight;
	float specularFactor = max( dot(reflectedLight, unitToCamera), 0);
	float dampedFactor = pow(specularFactor, shininess);
	vec3 specular = dampedFactor * specularConstant * lightColor;

	vec3 totalLight = ambient + diffuse + specular;
	fragColor = vec4(totalLight, 1.0) * texture(texture1, texCoord);
}

