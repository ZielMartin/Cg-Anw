// Does the phong illumination calculation once per vertex
#version 120

uniform sampler2D texture0;
uniform int useTexture;

varying vec3 vertex_color;
varying vec2 uv;

void main(void) {
    vec3 color = vertex_color;

    // mix in texture color if required
    if (useTexture != 0) color *= texture2D(texture0, uv.st).rgb;

    // set pixel color in OpenGL
    gl_FragColor = vec4(color, 1.0);
}
