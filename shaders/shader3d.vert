#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    Normal = normalize(aPos);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}