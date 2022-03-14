#version 420 core 

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 outTexCoord;

uniform mat4 modelMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform struct TexCoordIndex
{
    float tileWidth;
    float tileHeight;
    uint offsetX;
    uint offsetY;
} texCoordIndex;

/*
uniform TexCoordIndex
{
    float tileWidth;
    float tileHeight;
    uint offsetX;
    uint offsetY;
};*/

void main()
{
    //gl_Position = modelMatrix * vec4(inPosition.x, inPosition.y, 0.0f, 1.0f); 
    gl_Position = modelViewProjectionMatrix * vec4(inPosition.x, inPosition.y, 0.0f, 1.0f); 

    outTexCoord.x = inTexCoord.x * texCoordIndex.tileWidth + texCoordIndex.offsetX * texCoordIndex.tileWidth;
    outTexCoord.y = inTexCoord.y * texCoordIndex.tileHeight + texCoordIndex.offsetY * texCoordIndex.tileHeight;
}