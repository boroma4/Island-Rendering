#version 400

uniform vec3 lightPosition;

in vec3 interpolatedColor;
in vec3 interpolatedNormal;
in vec3 interpolatedPosition;

out vec4 frag_color;

void main(void) {

    float gamma = 2.2;
    vec3 viewerPosition = vec3(0.0);
    vec3 n = normalize(interpolatedNormal);
    vec3 l = normalize(lightPosition - interpolatedPosition);
    vec3 v = normalize(-interpolatedPosition);
    vec3 h = normalize(l + v);
    vec3 color = pow(interpolatedColor, vec3(gamma)) * (0.1 + max(0.0, dot(n, l))) + pow(max(0.0, dot(h, n)), 1000);
    color = pow(color, vec3(1.0 / gamma));

    frag_color = vec4(color, 1.0);
}
