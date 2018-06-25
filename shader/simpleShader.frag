// Does the phong illumination calculation once per vertex
#version 330 core

uniform sampler2D texture0;
uniform int useTexture;



in VertexData {
    vec2 uv;
    vec3 vertex_color;
    float radius;
} VertexIn;

out vec4 color;




void main(void) {
    vec3 vertex_color = VertexIn.vertex_color;

    // mix in texture vertex_color if required
    if (useTexture != 0) vertex_color *= texture2D(texture0, VertexIn.uv.st).rgb;


        if(VertexIn.radius > 0){
            // calculate normal from texture coordinates
            vec3 N;
            N.xy = gl_PointCoord* 2.0 - vec2(1.0);
            float mag = dot(N.xy, N.xy);
            if (mag > 1.0) discard;   // kill pixels outside circle
            N.z = sqrt(1.0-mag);

        }






        // set pixel color in OpenGL
        color = vec4(vertex_color, 1.0);




}







