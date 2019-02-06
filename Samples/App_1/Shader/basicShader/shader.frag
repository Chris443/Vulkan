#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}material;

layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragViewVec;
layout(location = 4) in vec3 fragLightVec;

layout(location = 0) out vec4 outColor;

vec3 lightColor = vec3(1.0);

void main() {
	outColor = texture(texSampler, fragTexCoord);

	vec3 N = normalize(fragNormal);
	vec3 L = normalize(fragLightVec);
	vec3 V = normalize(fragViewVec);
	vec3 R = reflect(L,N);

	vec3 ambient = vec3(texture(texSampler,fragTexCoord))*material.ambient;
	vec3 diffuse = max(dot(N,L),0.0)*vec3(texture(texSampler,fragTexCoord));
	vec3 specular =pow(max(dot(R,V),0.0),32.0)*vec3(1.8);

	outColor = vec4(ambient+diffuse+specular,1.0);
	}