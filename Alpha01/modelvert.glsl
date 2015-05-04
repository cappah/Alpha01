#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 rotate;
uniform mat4 scale;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * scale * rotate * vec4(position, 1.0f);
    TexCoords = texCoords;
}