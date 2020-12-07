#version 400

uniform vec2 frustum;
uniform vec3 lightDirection;
uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalMapTexture;
uniform float moveFactor;
uniform float waveStrength;

in vec3 interpolatedToCameraVector;
in vec3 interpolatedPosition;
in vec3 interpolatedColor;
in vec2 interpolatedUv;
in vec4 clipSpace;

out vec4 color;

void main(void) {

    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 refractionCoords = vec2(ndc.x, ndc.y);
    vec2 reflectionCoords = vec2(ndc.x, -ndc.y);

    float near = frustum.x;
    float far = frustum.y;

    float terrainDepth = texture(depthTexture, refractionCoords).r;
	float terrainDistance = 2.0 * near * far / (far + near - (2.0 * terrainDepth - 1.0) * (far - near));

	float fragmentDepth = gl_FragCoord.z;
	float fragmentDistance = 2.0 * near * far / (far + near - (2.0 * fragmentDepth - 1.0) * (far - near));

	float waterDepth = terrainDistance - fragmentDistance;
    float depthEffect = clamp(3.0 * waterDepth, 0.0, 1.0);

    vec2 distortedUv = texture2D(dudvTexture, vec2(interpolatedUv.x + moveFactor, interpolatedUv.y)).rg * 0.1;
	distortedUv = interpolatedUv + vec2(distortedUv.x, distortedUv.y + moveFactor);
	vec2 totalDistortion = (texture2D(dudvTexture, distortedUv).rg * 2.0 - 1.0) * waveStrength * depthEffect;

    refractionCoords += totalDistortion;
    reflectionCoords += totalDistortion;

    refractionCoords = clamp(refractionCoords, 0.001, 0.999);
    reflectionCoords.x = clamp(reflectionCoords.x, 0.001, 0.999);
    reflectionCoords.y = clamp(reflectionCoords.y, -0.999, -0.001);

    vec4 reflectionCol = texture2D(reflectionTexture, reflectionCoords).rgba;
    vec4 refractionCol = texture2D(refractionTexture, refractionCoords).rgba;

    vec4 normalMapColor = texture2D(normalMapTexture, distortedUv);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    vec3 n = normalize(normal);

    vec3 viewVector = normalize(interpolatedToCameraVector);
    float refractivity = pow(dot(viewVector, n), 0.5);

    vec4 mixedColor = mix(mix(reflectionCol, refractionCol, refractivity), vec4(interpolatedColor, 1.0), 0.25);

    float gamma = 2.2;
    vec3 viewerPosition = vec3(0.0);
    vec3 l = normalize(-lightDirection);
    vec3 v = viewVector;
    vec3 h = normalize(l + v);

    //TODO: use correct material constants
    float lighting = 0.1 + max(0.0, dot(n, l)) + pow(max(0.0, dot(h, n)), 1000);
    vec4 finalColor = pow(mixedColor, vec4(gamma)) * lighting *  depthEffect;
    color = pow(finalColor, vec4(1.0 / gamma));
    color.a = depthEffect;
    //color = vec4(waterDepth);
}
