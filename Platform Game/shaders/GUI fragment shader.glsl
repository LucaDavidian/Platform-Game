#version 420 core

in vec2 outTexCoord;
out vec4 fragColor;

uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);
uniform float opacity = 1.0f; 
uniform vec3 textColor = vec3(1.0f, 0.0f, 0.0f);
uniform bool hasTexture = false;
uniform bool isOver = false;

layout (binding = 0) uniform sampler2D textureSampler;     // samplers represent a texture and a set of sampling parameters (by default they refer to texture unit 0)

void main()
{
    if (hasTexture)
        fragColor = texture(textureSampler, outTexCoord);
    else
        fragColor = vec4(color, opacity);

    //if (isOver)
    //  fragColor += vec4(0.3f, 0.3f, 0.3f, 0.0f);
} 