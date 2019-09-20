#version 330 core
precision highp float;
uniform sampler2D tex0;

in vec2 Texcoord;

out vec4 col;
void main()
{
    vec2 uv = Texcoord;
    uv.y *= -1.0;
    col = texture(tex0, uv);
}
