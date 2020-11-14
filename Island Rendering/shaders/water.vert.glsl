#version 400

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

out vec3 interpolatedPosition;
out vec4 clipSpace;
out vec3 interpolatedColor;
out vec3 interpolatedNormal;
out vec2 interpolatedUv;

void main(void) {
    // probably would be better to send as uniform
    mat4 modelViewMatrix = viewMatrix * modelMatrix;
    mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));

    clipSpace = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    gl_Position = clipSpace;

    interpolatedNormal = normalize(normalMatrix * normal);
    interpolatedPosition = (modelViewMatrix * vec4(position, 1.0)).xyz;
    interpolatedColor = color;
    interpolatedUv = uv;
}
