#version 400

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

in vec3 position;
out vec3 texCoord;

void main(void) {

	texCoord = position;
	
	vec4 pos =  projectionMatrix * viewMatrix * vec4(position, 1.0);
	gl_Position = pos.xyww;
}