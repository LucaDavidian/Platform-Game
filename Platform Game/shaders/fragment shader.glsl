#version 420 core

in vec2 outTexCoord;
out vec4 fragColor;

uniform vec3 color = vec3(1.0f, 0.0f, 0.0f);

layout (binding = 0) uniform sampler2D textureSampler;     // samplers represent a texture and a set of sampling parameters (by default they refer to texture unit 0)
layout (binding = 1) uniform sampler2D textureSampler1;

uniform b
{
    vec3 uColor;
};

uniform bool hasTexture = false;

void main()
{
    vec4 texColor;
    
    if (hasTexture)
        texColor = texture(textureSampler, outTexCoord);
    else
        texColor = color;

    fragColor = vec4(color, texColor.r);
} 