#version 430 core

// In
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;

// Uniforms
layout(location = 0) uniform sampler2D u_sampler_diffuseMap;
uniform vec3 u_color;

// Out
layout (location = 0) out vec4 o_finalColor;

void main()
{
	// Texture color
	vec4 textureColor = texture(u_sampler_diffuseMap, f_uv);

	// Lighting
	vec3 ambientLighting = vec3(0.3f);
	vec3 lightDir = normalize(vec3(100.0f, 100.0f, 50.0f) - f_pos);
	vec3 diffuseLighting = vec3(max(dot(f_normal, lightDir), 0.0));
	
	// Final color
	o_finalColor.rgb = textureColor.rgb * u_color;
	o_finalColor.a = textureColor.a;
}
