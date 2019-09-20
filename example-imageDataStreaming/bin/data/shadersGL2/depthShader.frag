#version 120
precision highp float;
uniform sampler2D tex0;
void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.y *= -1.0;
    vec4 c = texture2D(tex0, uv);
    float coef = 75.0;
    vec3 col = vec3(0.0, 0.0, 0.0);


    if(c.r < 1.0){
      col.rgb = vec3(0.0, 0.0, c.r);
    }else if(c.r > 1.0 / 30.0 ){
      col.rgb = vec3(c.r, 0.0, 0.0);
    }
    gl_FragColor = vec4(vec3(col.b)*100.0, 1.0) + vec4(0.3, 0.3, 0.3, 0.0);
}
