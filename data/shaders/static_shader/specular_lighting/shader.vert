#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec3 lightPosition;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 surfaceNormal;
layout(location = 2) out vec3 toLightVector;
layout(location = 3) out vec3 toCameraVector;
layout(location = 4) out float visibility;

const float density = 0.007;
const float gradient = 1.5;

void main() {
	vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
	
	vec4 distanceCameraToObject = ubo.view * worldPosition;
	
    gl_Position = ubo.proj * distanceCameraToObject;
    fragTexCoord = inTexCoord;
	
	surfaceNormal = (ubo.model * vec4(normal, 0.0)).xyz;
	toLightVector = ubo.lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(ubo.view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	
	float distance = length(distanceCameraToObject.xyz * density);
	visibility = exp(-pow((distance * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0); //clamp to make sure value will be between 0,1 for out color of fragment shader
}