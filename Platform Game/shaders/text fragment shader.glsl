#version 420 core

in vec2 outTexCoord;
out vec4 fragColor;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);

layout (binding = 0) uniform sampler2D textureSampler;     // samplers represent a texture and a set of sampling parameters (by default they refer to texture unit 0)

void main()
{
	vec4 texColor = texture(textureSampler, outTexCoord); 
	fragColor = vec4(color, texColor.r);
} 