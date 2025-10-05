
#ifndef MY_OBJS_GRPX_ENG
#define MY_OBJS_GRPX_ENG

#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

class Base3D {
protected:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;

    virtual void generateGeometry() = 0;  // pure virt

    void setupBuffers();                  // shared
public:
    // Base3D();                               // calls generateGeometry + setupBuffers
    virtual ~Base3D();

    virtual void draw() const;            // virt, need not be ovridn
};


class Sphere : public Base3D {
protected:

    std::array<float, 3> center; 
    float radius; 
    unsigned int resolution;

    void draw() const override;
    void generateGeometry() override;

public:
    Sphere(
        std::array<float, 3>&& c, 
        float r, 
        unsigned int res);
};

#endif