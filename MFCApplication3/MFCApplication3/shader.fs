#version 330

//in vec4 Color;

out vec4 FragColor;

uniform vec3 Color;

void main()
{
    FragColor = vec4(Color, 1.0);
}