
#ifndef MY_OBJS_GRPX_ENG
#define MY_OBJS_GRPX_ENG

#include <glad/glad.h>
#include <GLFW3/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

class RenderObj{
protected:
    std::vector<glm::vec3> vertices;
    unsigned int VAO, VBO = 0;
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


public:
    virtual void draw(GLint locModel, GLint colorLoc) const = 0;
    virtual void setupBuffers() = 0;
    void updateModel(glm::mat4 mod);
    void updateColor(glm::vec3 col);

    virtual ~RenderObj() = default; // make sure to always have a destructor if planning to 
                                    // ..utilize polymorphism.
};

class Locus: public RenderObj{
private:
    size_t MAX_TRAJECTORY_POINTS = 500; 
public:
    Locus();

    void draw(GLint locModel, GLint colorLoc) const override final;
    void setupBuffers() override final;
    void updateLocus(glm::vec3& interpolated_position);

    ~Locus();
};


class Base3D: public RenderObj {
protected:
    
    unsigned int EBO;
    std::vector<unsigned int> indices;

    // do not change as 0 initialization is required
    Base3D(){
        VAO = 0;
        VBO = 0;
        EBO = 0;
        color = glm::vec3(1.0f, 1.0f, 0.2f);
        model = glm::mat4(1.0f);
    }
    virtual void generateGeometry() = 0;  // pure virt

    void setupBuffers();                  // shared
public:
    // Base3D();                               // calls generateGeometry + setupBuffers
    virtual ~Base3D();

    void draw(GLint locModel, GLint colorLoc) const override;            // use "final" if no further overriding required
};


class Sphere : public Base3D {
protected:

    glm::vec3 center; 
    float radius; 
    unsigned int resolution;

    // void draw() const override;
    void generateGeometry() override final;

public:
    Sphere(float r = 0.2, unsigned int res = 15, glm::vec3&& c = glm::vec3(0.f), glm::vec3&& col = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif