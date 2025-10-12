#ifndef MY_SHADERG_RPX_ENG
#define MY_SHADER_GRPX_ENG

#include <string>
#include <glad/glad.h>

class Shader {
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use() const;
    GLuint getID() const { return ID; }
private:
    std::string readFile(const char* filePath);
    GLuint compileShader(GLenum type, const char* src);
};


#endif