// Does the phong illumination calculation once per pixel
#version 120

uniform vec3 materialSpecular, lightSpecular, lightPosition;
uniform float materialShininess, constantAttenuation, linearAttenuation;
uniform int useTexture;
uniform sampler2D texture0;

varying vec3 diffuse, ambientGlobal, ambient, position, normal;
varying vec2 uv;

void main(void) {
    // do lighting computation
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - position);
    vec3 R = 2 * dot(L, N) * N - L;

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float attenuation = 1.0 / (constantAttenuation + length(L) * linearAttenuation);

    vec3 color = ambientGlobal;
    if (cosTheta > 0.0) {
        color += attenuation * (diffuse * cosTheta + ambient);
        color +=   attenuation
                 * materialSpecular
                 * lightSpecular
                 * pow(cosAlpha, materialShininess);
    }

    // mix in texture color if required
    if (useTexture != 0) color *= texture2D(texture0, uv.st).rgb;

    // set pixel color in OpenGL
    gl_FragColor = vec4(color, 1.0);
}
