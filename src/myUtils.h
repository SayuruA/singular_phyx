#ifndef MY_UTILS_PHY_ENG
#define MY_UTILS_PHY_ENG

#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <iostream>

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

std::vector<float> createSphere(float center, float radius){
    
}


#endif