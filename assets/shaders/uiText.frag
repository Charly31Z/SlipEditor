#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 color;

void main()
{
    //float depthValue = texture(texture1, TexCoord).r;
    //FragColor = vec4(vec3(depthValue), 1.0);
    //FragColor = texture(texture1, TexCoord);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
    FragColor = vec4(color, 1.0) * sampled;
}