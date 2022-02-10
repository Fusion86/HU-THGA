#version 430 core

// In variables
in vec3 f_pos;
in vec2 f_uv;
in vec3 f_normal;

// Textures
layout(location = 0) uniform sampler2D u_sampler_diffuse;
layout(location = 1) uniform sampler2D u_sampler_blendMapR;
layout(location = 2) uniform sampler2D u_sampler_blendMapG;
layout(location = 3) uniform sampler2D u_sampler_blendMapB;

// Uniforms
uniform vec2 u_uvRepeatR;
uniform vec2 u_uvRepeatG;
uniform vec2 u_uvRepeatB;
uniform bool u_blendmapped;
uniform vec3 u_color;

// Out variables
layout (location = 0) out vec4 o_finalColor;

void main()
{
    // Texture color
    vec4 textureColor;
    if(u_blendmapped)
    {
	    vec3 blendmapColor = texture(u_sampler_diffuse, f_uv).rgb;
	    vec3 rTextureColor = texture(u_sampler_blendMapR, f_uv * u_uvRepeatR).rgb * blendmapColor.r;
	    vec3 gTextureColor = texture(u_sampler_blendMapG, f_uv * u_uvRepeatG).rgb * blendmapColor.g;
	    vec3 bTextureColor = texture(u_sampler_blendMapB, f_uv * u_uvRepeatB).rgb * blendmapColor.b;
	    textureColor       = vec4(vec3(rTextureColor + gTextureColor + bTextureColor), 1.0f);
	}
	else
	{
		textureColor = texture(u_sampler_diffuse, f_uv);
		if(textureColor.a < 0.25f)
		{
			discard;
		}
	}

    // Lighting
    vec3 ambientLighting = vec3(0.2f);
    vec3 lightDir = normalize(vec3(0.0f, 100.0f, 25.0f) - f_pos);
    vec3 diffuseLighting = vec3(max(dot(f_normal, lightDir), 0.0));

    // Final color
    o_finalColor.rgb = textureColor.rgb * (ambientLighting + diffuseLighting) * u_color;
    o_finalColor.a = textureColor.a;
}
