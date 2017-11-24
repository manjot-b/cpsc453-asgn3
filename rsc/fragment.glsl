#version 410 core
uniform vec4 uColor;			// used for input from program (CPU)

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLight;
in vec3 toCamera;

uniform sampler2D modelTexture;
uniform sampler2D aoMap;
uniform int renderTexture;
uniform int renderAOMap;
uniform vec3 lightColor;
uniform vec4 lightConstants;

float ambientConstant;
float diffuseConstant;
float specularConstant;
float shininess;
vec4 textureColor;
vec4 aoMapColor;

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
	vec4 specular = vec4(dampedFactor * specularConstant * lightColor, 1.0);

	vec4 totalLight = vec4(ambient + diffuse, 1.0);

	//AMBIENT OCCLUSION	
	float greyVal = (texture(aoMap, texCoord)).x;
	vec4 gray = vec4(greyVal, greyVal, greyVal, 1.0);

	if (renderTexture == 1)		// true
		textureColor = texture(modelTexture, texCoord);
	else
		textureColor = vec4(1, 1, 1, 1);

	if (renderAOMap == 1)	// true
		aoMapColor = gray;
	else
		aoMapColor = vec4(1, 1, 1, 1);

	fragColor = totalLight * textureColor * aoMapColor + specular;
}

