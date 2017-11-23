#version 410 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform vec3 lightPosition;

out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 toLight;
out vec3 toCamera;

void main()
{
	vec4 worldPosition = model * vec4(aPosition, 1.0);
	gl_Position = perspective * view * worldPosition;
	texCoord = aTexture;

	surfaceNormal  = (model * vec4(aNormal, 0.0f)).xyz;
	toLight = lightPosition - worldPosition.xyz;

	toCamera = (inverse(view) * vec4(0, 0, 0, 1)).xyz;
}
