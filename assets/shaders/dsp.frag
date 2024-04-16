#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material {
	vec3 color;
    vec3 ambient;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
    FragColor = vec4(material.color, 0.25);
}