#version 430 core

layout(location = 0) in vec2 v_pos;
layout(location = 1) in vec2 v_uv;

uniform vec2 u_pos;
uniform vec2 u_size;

out vec2 f_uv;

void main()
{
    gl_Position = vec4((v_pos * u_size) + u_pos, 0.0f, 1.0f);
    f_uv = vec2(v_uv.x, -v_uv.y);
}
