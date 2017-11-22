#version 410 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
out vec2 texCoord;

void main()
{
	//vec4 trans = modelView * vec4(aPos, 1.0f);
	//gl_Position = vec4(trans.xyz, 1/scale);
	gl_Position = perspective * view * model * vec4(aPosition, 1.0);
	texCoord = aTexture;
}
