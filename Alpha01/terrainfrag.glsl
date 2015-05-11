#version 330 core
 
layout (location = 0) out vec4 color;

smooth in vec2 TexCoord;
smooth in vec3 Normal;

uniform sampler2D tex;



void main()
{
	vec3 normalized = normalize(Normal);

	color = texture(tex, TexCoord);
}