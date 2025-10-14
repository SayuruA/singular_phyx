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

    // Tetrahedron definition: 4 unique vertices, with per-vertex color
    // Vertex order: x,y,z, r,g,b
    // float vertices[] = {
    //      0.0f,  0.2f,  0.0f,   1.0f, 0.5f, 0.2f,   // V0 top (orange-ish)
    //     -0.2f, -0.2f,  0.2f,   0.2f, 0.7f, 1.0f,   // V1 front-left (blue-ish)
    //      0.2f, -0.2f,  0.2f,   0.2f, 1.0f, 0.4f,   // V2 front-right (green-ish)
    //      0.0f, -0.2f, -0.2f,   1.0f, 0.8f, 0.2f    // V3 back (yellow-ish)
    // };


    // unsigned int indices[] = {
    //     0,1,2,  // front face
    //     0,2,3,  // right face
    //     0,3,1,  // left face
    //     1,3,2   // bottom face (base)
    // };

    // sphere - orbiting object
    // float x =-3.0;
    // std::array<float, 3> center1 = {0.0f, 0.0f, 0.0f};
    // Sphere s1(std::move(center1), 0.1, 15);
    // Base3D& obj1 = s1;
    glm::vec3 start_pos1 = glm::vec3(-3.0f, 0.0f, 0.0f);
    glm::vec3 start_vel1 = glm::vec3(0.0f, 0.7f, 0.0f);
    float mass1 = 1.0f;
    float radius1 = 0.1f;
    Celestial c_earth = Celestial(start_pos1, start_vel1, mass1, radius1, true);


    // sphere - gravity point (static at origin)
    // std::array<float, 3> center_gravity = {0.0f, 0.0f, 0.0f};
    // Sphere s_gravity(std::move(center_gravity), 0.4, 15);
    // Base3D& obj_gravity = s_gravity;
    glm::vec3 start_pos2 = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 start_vel2 = glm::vec3(0.0f,0.0f,0.0f);
    float mass2 = 1e5;
    float radius2 = 0.3f;
    Celestial c_sun = Celestial(start_pos2, start_vel2, mass2, radius2, true);

    // physics object - state that will be updated
    // PhysObj p_obj1 = PhysObj(glm::vec3(x, 0.0f, 0.0f), glm::vec3(0.0f, 0.f, 0.8f), 1.0f); // 0.9 //1.4
    
    // create physics universe with gravity point at origin
    std::vector<Celestial*> c_objects = {&c_sun, &c_earth};
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -12.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                                800.0f / 600.0f, 
                                                0.1f, 100.0f);

    Universe_With_One_Star universe(std::move(c_objects), glm::vec3(0.0f), view, projection);

    // interpolation state
    // glm::vec3 prev_position = p_obj1.position;
    // glm::vec3 curr_position = p_obj1.position;
    // int frame_counter = 0;

    // trajectory tracking
    // const size_t MAX_TRAJECTORY_POINTS = 500;
    // std::vector<glm::vec3> trajectory_points;
    // trajectory_points.push_back(p_obj1.position);
    
    // trajectory VAO/VBO for line rendering
    // unsigned int traj_VAO, traj_VBO;
    // glGenVertexArrays(1, &traj_VAO);
    // glGenBuffers(1, &traj_VBO);
    
    // glBindVertexArray(traj_VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, traj_VBO);
    // glBufferData(GL_ARRAY_BUFFER, MAX_TRAJECTORY_POINTS * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    // glEnableVertexAttribArray(0);
    // glBindVertexArray(0);

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
    // std::array<GLint, 4> uniformLocations;
    // std::array<std::string, 4> uniformsArr;
    // for (std::string uniform :uniformsArr){

    // }

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

        // Update physics every..
        // const uint8_t update_every = 10;
        // const float FIXED_DT = 0.0080f;

        
        if (frame_counter % update_every == 0) {
        //     prev_position = curr_position;
        //     universe.linear_step(FIXED_DT);  // ~20 frames at 60fps ~= 0.33s
            universe.physx_update(FIXED_DT);
        //     curr_position = p_obj1.position;
        }
        

        // Interpolation factor [0, 1] within the 20-frame window
        // float alpha = (float) (frame_counter % update_every) / update_every;
        // glm::vec3 interpolated_position = glm::mix(prev_position, curr_position, alpha);


        // Record trajectory every frame (with minimum distance threshold)
        // if (trajectory_points.empty() || glm::distance(trajectory_points.back(), interpolated_position) > 0.05f) {
        //     trajectory_points.push_back(interpolated_position);
        //     if (trajectory_points.size() > MAX_TRAJECTORY_POINTS) {
        //         trajectory_points.erase(trajectory_points.begin());
        //     }
        // }

        
        

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
        // glm::mat4 model = glm::rotate(glm::mat4(1.0f),
        //                             time * glm::radians(100.0f),
        //                             glm::vec3(1.0f, 0.0f, 0.0f));
        // glm::mat4 model = glm::mat4(1.0f);
        // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -12.0f));
        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
        //                                         800.0f / 600.0f, 
        //                                         0.1f, 100.0f);
        
        
        
        // glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(locProj,  1, GL_FALSE, glm::value_ptr(projection));
        // glUniform3f(colorLoc, 1.0f, 1.0f, 0.2f);

        // glBindVertexArray(VAOs);
        // glDrawElements(GL_TRIANGLES,  sphere1.indices.size(), GL_UNSIGNED_INT, 0); // 4 faces * 3 indices = 12
        // glBindVertexArray(0);
        // obj_gravity.draw();

        // model = glm::translate(glm::mat4(1.0f), interpolated_position);


        // glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
        // glUniformMatrix4fv(locView,  1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(locProj,  1, GL_FALSE, glm::value_ptr(projection));
        // glUniform3f(colorLoc, 0.1f, 0.1f, 1.0f);

        // obj1.draw();

        // Draw trajectory
        // if (trajectory_points.size() > 1) {
        //     glBindBuffer(GL_COPY_WRITE_BUFFER, traj_VBO);
        //     glBufferSubData(GL_COPY_WRITE_BUFFER, 0, trajectory_points.size() * sizeof(glm::vec3), trajectory_points.data());
            
        //     glm::mat4 model_traj = glm::mat4(1.0f);
        //     glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model_traj));
        //     glUniform3f(colorLoc, 0.5f, 0.8f, 0.2f);  // greenish
            
        //     glBindVertexArray(traj_VAO);
        //     glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)trajectory_points.size());
        //     glBindVertexArray(0);
        // }
        universe.frame_update(frame_counter, update_every);
        universe.draw(locModel, locView, locProj, colorLoc);
        
        showFPS(window);
        glfwSwapBuffers(window);
        frame_counter++;

    }
    } // end of scoping.. cleanup will happen now as universe and shader gets deleted.
    // cleanup
    
    // glDeleteBuffers(1, &traj_VBO);
    // glDeleteVertexArrays(1, &traj_VAO);
    // Shader program deleted in Shader destructor
    // buffers should be deleted by base3d destructor
    // glDeleteBuffers(1, &VBOs);
    // glDeleteBuffers(1, &EBOs);
    // glDeleteVertexArrays(1, &VAOs);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
