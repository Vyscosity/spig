$input v_texcoord0 //v_color0, v_texcoord0 //in..

#include <bgfx_shader.sh>

//SAMPLER2D(s_texColor, 0);
SAMPLER2D(s_texColor, 0);

vec3 toLinear(vec3 _rgb)
{
    //return pow(_rgb, vec3(1.0 / 2.2));
    //return vec3(pow(_rgb.r, 2.2), pow(_rgb.g, 2.2), pow(_rgb.b, 2.2));
    return vec3(pow(_rgb.r, 1.2), pow(_rgb.g, 2.2), pow(_rgb.b, 2.2));
}

void main()
{
    vec4 color = texture2D(s_texColor, v_texcoord0);
    vec3 linearColor = toLinear(color.rgb);

    gl_FragColor.rgb = linearColor;
    gl_FragColor.a = 1.0;
}