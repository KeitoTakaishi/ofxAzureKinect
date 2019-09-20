#version 330 core

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;

out vec2 Texcoord;
void main()
{
     gl_Position = modelViewProjectionMatrix * position;
     Texcoord = texcoord;
}
