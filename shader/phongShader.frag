#version 330 core

uniform vec3 materialSpecular, lightSpecular, lightPosition;
uniform float materialShininess, constantAttenuation, linearAttenuation;
uniform int useTexture;
uniform sampler2D texture0;

in vec3 diffuse, ambientGlobal, ambient, position, normal;
in vec2 uv;

in VertexData {
    vec2 uv;
    vec3 vertex_color;
    float radius;
} VertexIn;

out vec4 color;

void main(void) {
    // do lighting computation
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - position);
    vec3 R = 2 * dot(L, N) * N - L;

    float cosTheta = max(dot(L, N), 0.0);
    float cosAlpha = max(dot(N, R), 0.0);

    float attenuation = 1.0 / (constantAttenuation + length(L) * linearAttenuation);

    vec3 tempColor = VertexIn.vertex_color + ambientGlobal;


    if (cosTheta > 0.0) {
        tempColor += attenuation * (diffuse * cosTheta + ambient);
        tempColor +=   attenuation
                 * materialSpecular
                 * lightSpecular
                 * pow(cosAlpha, materialShininess);
    }

    // mix in texture color if required
    //if (useTexture != 0) color *= texture2D(texture0, uv.st).rgb;

     if(VertexIn.radius > 0){
                vec3 N;
                N.xy = gl_PointCoord* 2.0 - vec2(1.0);
                float mag = dot(N.xy, N.xy);
                if (mag > 1.0) discard;   // kill pixels outside circle
                N.z = sqrt(1.0-mag);

            }

    // set pixel color in OpenGL
    color = vec4(tempColor, 1.0);
}
