uniform sampler2D atlas;
uniform mat4 transform_VP;

#ifdef _FRAGMENT_

uniform vec2 textSize;

in vec2 fragTexcoord;

// Ouput data
out vec4 color;

void main()
{
	//color = texture(atlas, fragTexcoord + (0.5f / textSize));
	color = texture(atlas, fragTexcoord);
}


#endif

#ifdef _VERTEX_

// Input vertex data, different for all executions of this shader.
layout(location = 1) in vec3 vertex;
layout(location = 2) in vec2 texture;
layout(location = 3) in vec3 normal;

out vec2 fragTexcoord;

void main()
{	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  transform_VP * vec4(vertex, 1.0);

	fragTexcoord = texture;
}

#endif