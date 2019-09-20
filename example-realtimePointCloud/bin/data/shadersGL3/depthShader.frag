#version 330 core
precision highp float;
uniform sampler2D tex0;

in vec2 Texcoord;

out vec4 col;

void main()
{
    vec2 uv = Texcoord;
    uv.y *= -1.0;
    vec4 c = texture(tex0, uv);
    float coef = 75.0;

    if(c.r < 1.0){
      col.rgb = vec3(0.0, 0.0, c.r);
    }else if(c.r > 1.0 / 30.0 ){
      col.rgb = vec3(c.r, 0.0, 0.0);
    }
    col = vec4(vec3(col.b)*100.0, 1.0) + vec4(0.3, 0.3, 0.3, 0.0);
}
