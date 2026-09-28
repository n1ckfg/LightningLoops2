// Adds the weighted blur layers on top of the scene.

uniform sampler2D tex0; // the scene
uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;
uniform sampler2D blur4;
uniform float weights[5];

varying vec2 vTexCoord;

void main() {
    vec3 bloom = texture2D(blur0, vTexCoord).rgb * weights[0]
               + texture2D(blur1, vTexCoord).rgb * weights[1]
               + texture2D(blur2, vTexCoord).rgb * weights[2]
               + texture2D(blur3, vTexCoord).rgb * weights[3]
               + texture2D(blur4, vTexCoord).rgb * weights[4];
    gl_FragColor = vec4(texture2D(tex0, vTexCoord).rgb + bloom, 1.0);
}
