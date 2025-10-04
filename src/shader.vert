#version 330 core
layout(location = 0) in vec3 aPos;       // vertex position
layout(location = 1) in vec3 aColor;     // per-vertex color

out vec3 vColor;                         // pass to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    // gl_Position expects clip-space coordinates. We use column-major matrices.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
