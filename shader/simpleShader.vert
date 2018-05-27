// Does the phong illumination calculation once per vertex
#version 120

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse, materialSpecular;
uniform vec3 lightAmbient, lightDiffuse, lightSpecular, lightPosition, lightGlobal;
uniform float materialShininess, constantAttenuation, linearAttenuation;

attribute vec3 vertex_position, vertex_normal;
attribute vec2 vertex_uv;

varying vec3 vertex_color;
varying vec2 uv;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);
    vec3 position = vec3(viewMatrix * modelMatrix * vertex);

    // compute base colors
    vec3 ambientGlobal = materialAmbient * lightGlobal;
    vec3 ambient = materialAmbient * lightAmbient;
    vec3 diffuse = materialDiffuse * lightDiffuse;

    // do the lighting computation
    vec3 N = normalize(normalMatrix * vertex_normal);
    vec3 L = normalize(lightPosition - position);
    vec3 R = 2 * dot(L, N) * N - L;

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float attenuation = 1.0 / (constantAttenuation + length(L) * linearAttenuation);

    vec3 color = ambientGlobal;
    if (cosTheta > 0.0) {
        color += attenuation * (diffuse * cosTheta + ambient);
        color +=  attenuation
                * materialSpecular
                * lightSpecular
                * pow(cosAlpha, materialShininess);
    }

    // pass variables
    uv = vertex_uv;
    vertex_color = color;

    // set vertex position in OpenGL
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;
}
