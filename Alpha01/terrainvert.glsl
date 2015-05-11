#version 330 core
  
layout (location = 0) in vec3 position; //vertex position in object space
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;

//uniforms
uniform mat4 MVP;					//combined modelview projection matrix
uniform ivec2 HALF_TERRAIN_SIZE;	//half terrain size
uniform sampler2D heightMapTexture;	//heightmap texture
uniform float scale;				//scale for the heightmap height
uniform float half_scale;			//half of the scale

void main()
{   
	//extract height from the heightmap texture for the given vertex position
	//rescale this height using the scale and half_scale uniforms
	float height = texture(heightMapTexture, position.xz).r * scale - half_scale;

	//rescale the terrain vertices to be in -HALF_TERRAIN_SIZE to HALF_TERRAIN_SIZE
	//in width and depth
	vec2 pos = (position.xz * 2.0 - 1) * HALF_TERRAIN_SIZE;

	//mulitply the modelview projection matrix with the scaled position and height
	gl_Position = MVP * vec4(pos.x, height, pos.y, 1);	
	
	TexCoord = texCoord;
	Normal = normal;		
}