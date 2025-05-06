#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inUV;

layout (binding = 0) uniform UBO 
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) out vec3 outUVW;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main() 
{
	outUVW = inPos;
	mat4 viewMat = mat4(mat3(ubo.view));
	gl_Position = ubo.proj * viewMat * ubo.model * vec4(inPos.xyz, 1.0);
}
