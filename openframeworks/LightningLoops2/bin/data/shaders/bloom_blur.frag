// Separable 9-tap gaussian blur, run once along x and once along y.
// Uses linear filtering to read two texels per sample.

uniform sampler2D tex0;
uniform vec2 direction; // one texel along the blur axis

varying vec2 vTexCoord;

void main() {
    vec4 sum = texture2D(tex0, vTexCoord) * 0.2270270270;
    sum += texture2D(tex0, vTexCoord + direction * 1.3846153846) * 0.3162162162;
    sum += texture2D(tex0, vTexCoord - direction * 1.3846153846) * 0.3162162162;
    sum += texture2D(tex0, vTexCoord + direction * 3.2307692308) * 0.0702702703;
    sum += texture2D(tex0, vTexCoord - direction * 3.2307692308) * 0.0702702703;
    gl_FragColor = sum;
}
