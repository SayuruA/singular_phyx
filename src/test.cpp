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
#include "Objects.h"
#include "myUtils.h"
#include "phyx.h"

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

    GLFWwindow* window = glfwCreateWindow(1400, 1800, "Tetrahedron - OpenGL 3.3", nullptr, nullptr);
    // GLFWwindow* window;
    // // GLFWmonitor* monitor = glfwGetPrimaryMonitor(); // Get the primary monitor
    // // const GLFWvidmode* mode = glfwGetVideoMode(monitor); // Get its video mode
    // // window = glfwCreateWindow(mode->width, mode->height, "Fullscreen Window", monitor, NULL);
    
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
    glDisable(GL_CULL_FACE);


    {

    glm::vec3 start_pos1 = glm::vec3(-3.0f, 0.0f, 0.0f);
    glm::vec3 start_vel1 = glm::vec3(0.0f, 0.7f, 0.0f);
    float mass1 = 1.0f;
    float radius1 = 0.1f;
    Celestial c_earth = Celestial(start_pos1, start_vel1, mass1, radius1, true);

    // Create a set of orbiting planets at the same location as c_earth but at different speeds
    std::vector<Celestial> orbiting_planets;
    float speeds[] = {0.5f, 0.9f, 1.2f, 1.5f};
    float masses[] = {0.8f, 1.2f, 0.6f, 1.0f};
    float radii[] = {0.08f, 0.12f, 0.07f, 0.10f};
    
    for (int i = 0; i < 4; ++i) {
        glm::vec3 planet_vel = glm::vec3(0.0f, speeds[i], 0.0f);
        orbiting_planets.emplace_back(start_pos1, planet_vel, masses[i], radii[i], true);
    }

    glm::vec3 start_pos2 = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 start_vel2 = glm::vec3(0.0f,0.0f,0.0f);
    float mass2 = 1e5;
    float radius2 = 0.3f;
    Celestial c_sun = Celestial(start_pos2, start_vel2, mass2, radius2, true);

    // create physics universe with gravity point at origin
    std::vector<Celestial*> c_objects = {&c_sun, &c_earth};
    
    // Add orbiting planets to the universe
    for (auto& planet : orbiting_planets) {
        c_objects.push_back(&planet);
    }
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -12.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                800.0f / 600.0f, 
                                                0.1f, 100.0f);

    Universe_With_One_Star universe(std::move(c_objects), glm::vec3(0.0f), view, projection);
    
    Shader shader3d("../shaders/shader3d.vert", "../shaders/shader3d.frag");


    if (!shader3d.getID()) {
        std::cerr << "Failed to create shader program\n";
        return -1;
    }

    // get uniform locations
    GLint locModel = glGetUniformLocation(shader3d.getID(), "model");
    GLint locView  = glGetUniformLocation(shader3d.getID(), "view");
    GLint locProj  = glGetUniformLocation(shader3d.getID(), "projection");
    GLint colorLoc = glGetUniformLocation(shader3d.getID(), "sphereColor");
   
    // main loop
    // update physics for every 20 th frame, linearly move in between
    const int update_every = 10;
    const float FIXED_DT = 0.0080f; // can use an arbitrary 
									//..value that can keep other parameters
									//..within convenient limits             
    int frame_counter = 0; // keep at 0 to do a phys update in the very
									//..first iteration

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        // compute simple transforms
        float time = (float)glfwGetTime();
        
        if (frame_counter % update_every == 0) {

            universe.physx_update(FIXED_DT);

        }
        
        // clear color + depth
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 4.0f/3.0f;

        // draw
        shader3d.use();
        // send matrices (column-major order); 1 = count; GL_FALSE = do not transpose
        // Create transformation matrices
        float timeValue = glfwGetTime();
 
        universe.frame_update(frame_counter, update_every);
        universe.draw(locModel, locView, locProj, colorLoc);
        
        showFPS(window);
        glfwSwapBuffers(window);
        frame_counter++;

    }
    } // end of scoping to call destructors.. cleanup will happen now as universe and shader gets deleted.
    // cleanup
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
