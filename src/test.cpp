// tetrahedron.cpp
// Single-file minimal OpenGL 3.3 example using GLFW + GLAD
// Renders a colored spinning tetrahedron (per-vertex colors interpolated across faces)

#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <cmath>
#include <vector>

// --------------------------------------
#include "Shader.h"
#include "myUtils.h"

int main() {
    // std::cout << "WTF??";

    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    // Request OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Tetrahedron - OpenGL 3.3", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GL functions via glad (you said it's linked)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Enable depth test (important for 3D)
    glEnable(GL_DEPTH_TEST);

    // Tetrahedron definition: 4 unique vertices, with per-vertex color
    // Vertex order: x,y,z, r,g,b
    float vertices[] = {
         0.0f,  0.2f,  0.0f,   1.0f, 0.5f, 0.2f,   // V0 top (orange-ish)
        -0.2f, -0.2f,  0.2f,   0.2f, 0.7f, 1.0f,   // V1 front-left (blue-ish)
         0.2f, -0.2f,  0.2f,   0.2f, 1.0f, 0.4f,   // V2 front-right (green-ish)
         0.0f, -0.2f, -0.2f,   1.0f, 0.8f, 0.2f    // V3 back (yellow-ish)
    };


    unsigned int indices[] = {
        0,1,2,  // front face
        0,2,3,  // right face
        0,3,1,  // left face
        1,3,2   // bottom face (base)
    };

    // Create VAO, VBO, EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind & fill
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // attribute 0: position (vec3) -> offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // attribute 1: color (vec3) -> offset 3 * float
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    Shader shader("../src/shader.vert", "../src/shader.frag");
    if (!shader.getID()) {
        std::cerr << "Failed to create shader program\n";
        return -1;
    }
    // get uniform locations
    GLint locModel = glGetUniformLocation(shader.getID(), "model");
    GLint locView  = glGetUniformLocation(shader.getID(), "view");
    GLint locProj  = glGetUniformLocation(shader.getID(), "projection");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // clear color + depth
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // compute simple transforms
        float time = (float)glfwGetTime();
      
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 4.0f/3.0f;

        // draw
        shader.use();
        // send matrices (column-major order); 1 = count; GL_FALSE = do not transpose
        // Create transformation matrices
        float timeValue = glfwGetTime();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                    time * glm::radians(20.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
        // glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                800.0f / 600.0f, 
                                                0.1f, 100.0f);
        
        
        
        glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(locProj,  1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // 4 faces * 3 indices = 12
        glBindVertexArray(0);
        showFPS(window);
        glfwSwapBuffers(window);
    }

    // cleanup
    // Shader program deleted in Shader destructor
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
