#version 330

in  vec3 VertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(VertexColor, 1);
}