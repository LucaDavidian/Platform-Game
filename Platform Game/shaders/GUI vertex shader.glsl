#version 420 core 

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 outTexCoord;

uniform mat4 modelMatrix;

void main()
{
    gl_Position = modelMatrix * vec4(inPosition.x, inPosition.y, 0.0f, 1.0f); 
    
    outTexCoord = inTexCoord;
}