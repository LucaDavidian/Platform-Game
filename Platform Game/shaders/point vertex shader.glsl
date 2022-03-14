#version 420 core 

layout (location = 0) in vec3 inPosition;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    gl_Position = modelViewProjectionMatrix * vec4(inPosition.x, inPosition.y, 0.0f, 1.0f); 
}