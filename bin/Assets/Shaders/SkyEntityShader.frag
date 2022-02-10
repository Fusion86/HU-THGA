#version 430 core

in vec3 f_uv;

layout (location = 0) out vec4 o_finalColor;

uniform samplerCube u_sampler;

void main()
{
    o_finalColor.rgb = texture(u_sampler, f_uv).rgb;
    o_finalColor.a   = 1.0f;
}
