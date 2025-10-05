#version 330 core

in vec3 Normal;
out vec4 FragColor;

uniform vec3 sphereColor;


void main()
{   
    
    // Map [-1,1] → [0,1]
    vec3 shade = Normal * 0.5 + 0.5;
    FragColor = vec4(sphereColor * shade, 1.0);
}