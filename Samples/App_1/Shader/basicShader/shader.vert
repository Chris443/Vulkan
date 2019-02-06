#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragViewVec;
layout(location = 4) out vec3 fragLightVec;


layout(binding = 0) uniform UBO
{
	mat4 model;
	mat4 view;
	mat4 projection;
	vec3 lightPos;
} ubo;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(pos,1.0);
	vec4 worldPos = ubo.model * vec4(pos,1.0);

	fragTexCoord = tex;
	fragNormal = mat3(ubo.model) * nor;
	fragViewVec = vec3(ubo.view * worldPos);
	fragLightVec = ubo.lightPos - vec3(worldPos);
	gl_Position.y = -gl_Position.y;
}
