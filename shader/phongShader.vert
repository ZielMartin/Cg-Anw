#version 330 core


uniform mat4 projectionMatrix, viewMatrix, modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 materialAmbient, materialDiffuse;
uniform vec3 lightAmbient, lightDiffuse, lightPosition, lightGlobal;
uniform vec3 cameraPos;


in vec3 vertex_position, vertex_normal, v_color;
in vec2 vertex_uv;
in float radius_attr;

out vec3 ambientGlobal, ambient, diffuse, position, normal;

out VertexData {
    vec2 uv;
    vec3 vertex_color;
    float radius;
} VertexOut;

void main(void) {
    vec4 vertex = vec4(vertex_position, 1.0);

    // transform normal and position for fragment shader
    normal = normalize(normalMatrix * vertex_normal);
    position = vec3(viewMatrix * modelMatrix * vertex);

    // base colors don't change per pixel - can compute now
    ambient = materialAmbient * lightAmbient;
    diffuse = materialDiffuse * lightDiffuse;
    ambientGlobal = materialAmbient * lightGlobal;



    float distance = length(vertex_position - cameraPos);

    gl_PointSize =  radius_attr/distance;

    // set vertex position in OpenGL
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertex;

    // pass variables
    VertexOut.uv = vertex_uv;
    VertexOut.vertex_color = v_color;
    VertexOut.radius = radius_attr;
}
