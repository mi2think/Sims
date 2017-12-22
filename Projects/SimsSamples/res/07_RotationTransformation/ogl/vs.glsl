#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

uniform mat4 gMatWorld;

out vec3 VertexColor;

void main()
{
	gl_Position = gMatWorld * vec4(Position, 1);
	VertexColor = Color;
}
