#version 430 core

// In
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 v_uv;
layout(location = 2) in vec3 v_normal;

// Uniforms
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform float u_uvPart;
uniform float u_uvOffset;

// Out
out vec3 f_pos;
out vec2 f_uv;
out vec3 f_normal;

void main()
{
    // In
    vec4 worldSpacePos = u_modelMatrix * vec4(v_pos, 1.0f);
    vec4 viewSpacePos  = u_viewMatrix * worldSpacePos;
    vec4 clipSpacePos  = u_projectionMatrix * viewSpacePos;

    // GLSL
    gl_Position = clipSpacePos;

    // Out
    f_pos = worldSpacePos.xyz;
    f_uv = vec2(u_uvOffset.x + (v_uv.x*u_uvPart.x), -v_uv.y);
    f_normal = normalize(mat3(inverse(transpose(u_modelMatrix))) * v_normal);
}
