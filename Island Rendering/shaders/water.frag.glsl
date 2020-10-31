#version 400

uniform vec3 lightPosition;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform float moveFactor;

in vec3 interpolatedPosition;
in vec3 interpolatedColor;
in vec3 interpolatedNormal;
in vec2 interpolatedUv;
in vec4 clipSpace;

out vec4 color;

const float waveStrength = 0.1;

void main(void) {

    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 refractionCoords = vec2(ndc.x, ndc.y);
    vec2 reflectionCoords = vec2(ndc.x, -ndc.y);

    vec2 distortion1 = (texture2D(dudvTexture, vec2(interpolatedUv.x + moveFactor, interpolatedUv.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion2 = (texture2D(dudvTexture, vec2(-interpolatedUv.x + moveFactor, interpolatedUv.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    vec2 totalDistortion = distortion1 + distortion2;

    refractionCoords += totalDistortion;
    reflectionCoords += totalDistortion;

    refractionCoords = clamp(refractionCoords, 0.001, 0.999);
    reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec4 reflectionCol = texture2D(reflectionTexture, reflectionCoords).rgba;
    vec4 refractionCol = texture2D(refractionTexture, refractionCoords).rgba;

    float gamma = 2.2;
    vec3 viewerPosition = vec3(0.0);
    vec3 n = normalize(interpolatedNormal);
    vec3 l = normalize(lightPosition - interpolatedPosition);
    vec3 v = normalize(-interpolatedPosition);
    vec3 h = normalize(l + v);
    vec4 mixedColor = mix(mix(reflectionCol, refractionCol, 0.5), vec4(interpolatedColor, 1.0), 0.3);
    float lighting = 0.1 + max(0.0, dot(n, l)) + pow(max(0.0, dot(h, n)), 1000);
    vec4 finalColor = pow(mixedColor, vec4(gamma)) * lighting;
    color = pow(finalColor, vec4(1.0 / gamma));
}
