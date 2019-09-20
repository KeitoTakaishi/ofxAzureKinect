#version 120
precision highp float;
// this is how we receive the texture
uniform sampler2D tex0;
void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    uv.y *= -1.0;
    vec4 c = texture2D(tex0, uv);
    float coef = 75.0;
    vec3 col = vec3(0.0, 0.0, 0.0);

    float near = 70.0;
    float mid = 50.0;
    float far = 35.0;

    /*
    初めの16bitはbgで表現する
    次gr
    最後rのみで表現する

    //距離の判別
    5m -> 65535 (=1.0)
    1:21845 (=0.3)
    2:43690 (=0.6)
    3:65535 (=0.9)
    */

    if(c.r < 1.0){
      col.rgb = vec3(0.0, 0.0, c.r);
    }else if(c.r > 1.0 / 30.0 ){
      col.rgb = vec3(c.r, 0.0, 0.0);
    }
    gl_FragColor = vec4(vec3(col.b)*100.0, 1.0) + vec4(0.3, 0.3, 0.3, 0.0);
}
