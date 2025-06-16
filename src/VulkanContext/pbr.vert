#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inTexCoord;
layout (location = 3) in vec3 inNormal;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 camPos;
	vec4 lights[4];
	vec4 rough;
	vec4 color;
} ubo;

layout (location = 0) out vec3 outWorldPos;
layout (location = 1) out vec3 outNormal;

void main() 
{
  	 gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	vec3 locPos = vec3(ubo.model * vec4(inPosition, 1.0));
	outWorldPos = locPos ;
	outNormal = mat3(ubo.model) * inNormal;

}
