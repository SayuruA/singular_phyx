#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertCode = readFile(vertexPath);
    std::string fragCode = readFile(fragmentPath);

    if (vertCode.empty() || fragCode.empty()) {
        std::cerr << "Shader source empty. Check file paths.\n";
        ID = 0;
        throw;
    }


    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    GLuint vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
    GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);
    
    if (!vertex || !fragment) {
        ID = 0;
        return;
    }
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "Program link error:\n" << infoLog << std::endl;
        ID = 0;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (ID) glDeleteProgram(ID);
    std::cout << "Shader program deleted successfully\n";
}

void Shader::use() const {
    if (ID) glUseProgram(ID);
}

std::string Shader::readFile(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    if (file) {
        buffer << file.rdbuf();
        file.close();
    } else {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
    }
    return buffer.str();
}

GLuint Shader::compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
