#version 430 core

in vec2 f_uv;

uniform sampler2D u_sampler_diffuse;

uniform vec3 u_color;

layout (location = 0) out vec4 o_finalColor;

void main()
{
    vec4 texColor = texture(u_sampler_diffuse, f_uv);
    o_finalColor.rgb = texColor.rgb * u_color;
    o_finalColor.a = texColor.a;
}
