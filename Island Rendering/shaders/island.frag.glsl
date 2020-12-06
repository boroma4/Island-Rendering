#version 400

uniform vec3 lightDirection;

in vec3 interpolatedColor;
in vec3 interpolatedNormal;
in vec3 interpolatedPosition;

out vec4 frag_color;

void main(void) {

    float gamma = 2.2;
    vec3 viewerPosition = vec3(0.0);
    vec3 n = normalize(interpolatedNormal);
    vec3 l = normalize(-lightDirection);
    vec3 v = normalize(-interpolatedPosition);
    vec3 h = normalize(l + v);
    vec3 color = pow(interpolatedColor, vec3(gamma)) * (0.1 + max(0.0, dot(n, l))) + pow(max(0.0, dot(h, n)), 1000);
    //vec3 color = pow(vec3(gl_FragCoord.x,0.0,gl_FragCoord.y), vec3(gamma)) * (0.1 + max(0.0, dot(n, l))) + pow(max(0.0, dot(h, n)), 1000);
    color = pow(color, vec3(1.0 / gamma));

    vec2 st = gl_FragCoord.xy/vec2(900,600);
    //frag_color = vec4(vec3(1.0,0.0,1.0), 1.0);
    //gl_FragColor = vec4(vec3(st.x,0.0,st.y), 1.0);
    frag_color = vec4(color, 1.0);
}
