#version 120
precision highp float;
uniform sampler2D tex0;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.y *= -1.0;
    vec4 c = texture2D(tex0, uv);
    gl_FragColor = c;
}
