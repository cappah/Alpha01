#version 420 core

in vec3 position;
in vec4 color;
out vec4 Color;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;


void main()
{ 
	gl_Position = Projection * View * Model * vec4(position, 1.0);
	Color = color;
}