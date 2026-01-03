#ifndef MY_CAMERA_GRPX_ENG
#define MY_CAMERA_GRPX_ENG

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW3/glfw3.h>
#include <iostream>

class Camera {
private:
    glm::vec3 position;
    glm::vec3 target;
    
    // Mouse input tracking
    double last_mouse_x = 0.0;
    double last_mouse_y = 0.0;
    bool right_mouse_pressed = false;
    
    // Sensitivity parameters
    float pan_sensitivity = 0.02f;
    float zoom_sensitivity = 0.5f;
    float min_distance = 2.0f;
    float max_distance = 50.0f;

public:
    Camera(glm::vec3 initial_pos = glm::vec3(-6.0f, 4.0f, -10.0f), 
           glm::vec3 target_pos = glm::vec3(0.0f, 0.0f, 0.0f))
        : position(initial_pos), target(target_pos) {}

    // Called from mouse button callback (event-driven)
    void onMouseButton(int button, int action) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            right_mouse_pressed = (action == GLFW_PRESS);
            // std::cout << "Mouse button callback: right button " << (right_mouse_pressed ? "PRESSED" : "RELEASED") << "\n";
        }
    }

    // Update camera based on mouse input
    void updateMouse(GLFWwindow* window) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        
        if (right_mouse_pressed) {
            // Calculate mouse delta
            double delta_x = mouse_x - last_mouse_x;
            double delta_y = mouse_y - last_mouse_y;
            
            std::cout << "Panning with delta: (" << delta_x << ", " << delta_y << ")\n";
            
            // Pan camera based on mouse movement
            glm::vec3 forward = glm::normalize(position - target);
            glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
            glm::vec3 up = glm::cross(forward, right);
            
            // Apply panning
            position += right * (float)delta_x * pan_sensitivity;
            position += up * (float)(-delta_y) * pan_sensitivity;
        }
        
        last_mouse_x = mouse_x;
        last_mouse_y = mouse_y;
    }

    // Handle scroll wheel for zoom
    void scroll(double yoffset) {
        glm::vec3 direction = glm::normalize(position - target);
        float distance = glm::length(position - target);
        
        // Zoom by moving closer or farther along the direction vector
        distance -= (float)yoffset * zoom_sensitivity;
        
        // Clamp distance
        if (distance < min_distance) distance = min_distance;
        if (distance > max_distance) distance = max_distance;
        
        position = target + direction * distance;
    }

    // Get the view matrix
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getTarget() const { return target; }

    // Setters
    void setPosition(glm::vec3 pos) { position = pos; }
    void setTarget(glm::vec3 tgt) { target = tgt; }
};

// Global camera instance and callback functions for GLFW
Camera* g_camera = nullptr;

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_camera) {
        g_camera->scroll(yoffset);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (g_camera) {
        std::cout<<"pressed" << button << ' ';
        g_camera->onMouseButton(button, action);
    }
}

#endif
