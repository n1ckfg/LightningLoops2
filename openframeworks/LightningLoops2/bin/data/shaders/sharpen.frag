// 3x3 sharpen kernel, mixed 20% over the original.
//  0 -1  0
// -1  5 -1
//  0 -1  0

uniform sampler2D tex0;
uniform vec2 texelSize;

varying vec2 vTexCoord;

void main() {
    vec4 origColor = texture2D(tex0, vTexCoord);

    vec4 sum = origColor * 5.0;
    sum -= texture2D(tex0, vTexCoord + vec2(0.0, -texelSize.y));
    sum -= texture2D(tex0, vTexCoord + vec2(-texelSize.x, 0.0));
    sum -= texture2D(tex0, vTexCoord + vec2(texelSize.x, 0.0));
    sum -= texture2D(tex0, vTexCoord + vec2(0.0, texelSize.y));

    gl_FragColor = (origColor * 0.8) + (sum * 0.2);
}
