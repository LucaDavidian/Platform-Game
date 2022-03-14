#version 420 core

in vec2 outTexCoord;
out vec4 fragColor;

layout (binding = 0) uniform sampler2D textureSampler;     // samplers represent a texture and a set of sampling parameters (by default they refer to texture unit 0)

uniform bool hasTexture = false;
uniform vec3 color = vec3(1.0f, 0.0f, 0.0f);
uniform bool isOver = false;

void main()
{
    vec4 texColor;

    if (hasTexture)
        texColor = texture(textureSampler, outTexCoord);
    else
        texColor = vec4(color, 1.0);

    fragColor = texColor;

    if (isOver)
        fragColor += vec4(0.3f, 0.3f, 0.3f, 0.0f);
} 