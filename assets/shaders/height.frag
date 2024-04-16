#version 410 core

in float Height;

out vec4 FragColor;

void main()
{
    float h = (Height + 16)/64.0f;	// shift and scale the height into a grayscale value
    FragColor = vec4(h, h, h, 1.0);
}