#version 400

uniform samplerCube cubeMap;

in vec3 texCoord;
out vec4 color;

void main(void) {
	color = texture(cubeMap, texCoord);
}
 