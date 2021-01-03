#version 400

uniform vec3 lightDirection;

uniform sampler2D sandTexture;
uniform sampler2D hillsTexture;

in vec3 interpolatedColor;
in vec3 interpolatedNormal;
in vec3 interpolatedPosition;

out vec4 frag_color;

void main(void) {


    float upperLimit = 0.35;
	float lowerLimit = 0.20;
    float alpha = clamp(interpolatedPosition.y - lowerLimit, 0, upperLimit - lowerLimit) / (upperLimit - lowerLimit);

    vec3 texColor = mix(texture2D(sandTexture, interpolatedPosition.xz), texture2D(hillsTexture, interpolatedPosition.xz), alpha).xyz;

    float gamma = 2.2;
    vec3 viewerPosition = vec3(0.0);
    vec3 n = normalize(interpolatedNormal);
    vec3 l = normalize(-lightDirection);
    vec3 v = normalize(-interpolatedPosition);
    vec3 h = normalize(l + v);
    vec3 color = pow(texColor, vec3(gamma)) * (0.1 + max(0.0, dot(n, l))); //+ pow(max(0.0, dot(h, n)), 1000);
    color = pow(color, vec3(1.0 / gamma));
    frag_color = vec4(color, 1.0); 

}
