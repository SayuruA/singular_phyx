#include <glad/glad.h>
#include <glfw3/glfw3.h>
#include <iostream>

int main(){
    std::cout << "Test" << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
    }


    return 0;
}