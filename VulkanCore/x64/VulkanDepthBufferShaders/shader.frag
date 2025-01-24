#version 450

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

void main() {
 outColor = texture(texSampler, fragTexCoord);
 outColor = texture(texSampler, fragTexCoord * 2.0);
 outColor = vec4(inColor * texture(texSampler,
	fragTexCoord).rgb, 1.0);
}