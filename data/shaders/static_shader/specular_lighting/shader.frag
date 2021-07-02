#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout (binding = 2) uniform Light 
{
	vec3 lightColor;
	float shineDamper;
	float reflectivity;
} light;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 surfaceNormal;
layout(location = 2) in vec3 toLightVector;
layout(location = 3) in vec3 toCameraVector;
layout(location = 4) in float visibility;

layout(location = 0) out vec4 outColor;

const float ambient = 0.0f;
const vec3 skyColor = vec3(0.5, 0.5, 0.5); // todo

void main() {
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, ambient);
	vec3 diffuse = brightness * light.lightColor;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitVectorToCamera;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float speculatFactor = dot(reflectedLightDirection, unitVectorToCamera);
	speculatFactor = max(speculatFactor, 0.0);

	float dampedFactor = pow(speculatFactor, light.shineDamper);
	vec3 finalSpecular = dampedFactor * light.reflectivity * light.lightColor;

	vec4 textureColor = texture(texSampler, fragTexCoord);
	
    vec4 outColorcc = vec4(diffuse, 1.0) * textureColor + vec4(finalSpecular, 1.0);
	outColor = mix(vec4(skyColor, 1.0), outColorcc, visibility);
}