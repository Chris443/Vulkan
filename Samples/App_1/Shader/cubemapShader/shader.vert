#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 now;

layout (binding = 0) uniform UBO 
{
	mat4 model;
	mat4 projection;
} ubo;

layout (location = 0) out vec3 tex;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	tex = pos;
	tex*=-1.0f;

	gl_Position = ubo.projection * ubo.model * vec4(pos.xyz, 1.0);
}