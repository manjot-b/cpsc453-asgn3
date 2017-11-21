#version 410 core
out vec4 fragColor;
uniform vec4 uColor;			// used for input from program (CPU)

in vec2 texCoord;
uniform sampler2D texture1;

void main()
{
	fragColor = vec4(0.75, 0.75, 1, 1.0);
	//fragColor = texture(texture1, texCoord);

}

