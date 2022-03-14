#version 420 core

out vec4 fragColor;

uniform vec3 color = vec3(1.0f, 0.0f, 0.0f);

void main()
{
    fragColor = vec4(color, 1.0f);
} 