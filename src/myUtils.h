#ifndef MY_UTILS_GRPX_ENG
#define MY_UTILS_GRPX_ENG

#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

void showFPS(GLFWwindow* window) {
    static double previousTime = glfwGetTime();
    static int frameCount = 0;

    double currentTime = glfwGetTime();
    double delta = currentTime - previousTime;
    frameCount++;

    // Update every 1 second
    if (delta >= 1.0) {
        double fps = double(frameCount) / delta;
        std::string title = "OpenGL Window - FPS: " + std::to_string((int)fps);
        glfwSetWindowTitle(window, title.c_str());

        frameCount = 0;
        previousTime = currentTime;
    }
}

/*
class Sphere{
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;



    // use move to prevent unnessary copying
    // '&&' makes sure constructor only accepts rvalues 
    Sphere(std::vector<float>&& v, std::vector<unsigned int>&& i)
        : vertices(std::move(v)), indices(std::move(i)) {}
    

};


Sphere createSphere(std::array<float, 3> center, float radius, unsigned int resolution){


    // This returns vertices and indices arrays (as one list) for a sphere having center and radius given.
    // resoultion is the number of splits taken in the angle in both directions

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

    unsigned int stacks  = resolution;
    unsigned int sectors = resolution;

    float stackStep  = PI / stacks;
    float sectorStep = 2 * PI / sectors;

    // ---------- Generate vertices ----------
    for (unsigned int i = 0; i <= stacks; ++i)
    {
        float stackAngle = PI / 2 - i * stackStep; // from +pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);
        float z  = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertices.push_back(x + center[0]);
            vertices.push_back(y + center[1]);
            vertices.push_back(z + center[2]);
        }
    }

    // ---------- Generate indices ----------
    for (unsigned int i = 0; i < stacks; ++i)
    {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // top stack
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // bottom stack
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return Sphere{ std::move(vertices), std::move(indices) };
}
*/

#endif