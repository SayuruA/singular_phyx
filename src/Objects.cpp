#include "Objects.h"
#include <stdexcept>

void Base3D::setupBuffers(){
    // check vertices and indices validity - raise error if not
    if (vertices.empty() or indices.empty()){
        throw std::invalid_argument("Requested buffer init with empty data vectors!");
    }

    // check vao is already set - return early if so
    if (VAO != 0) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind & fill
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float* verts = vertices.data();
    unsigned int* inds = indices.data();

    // note to self - better way to take the size than vertices.size() * sizeof(float) ??? - issue - dtype hardcoded
    glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), inds, GL_STATIC_DRAW);

    // attribute 0: position (vec3) -> offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Base3D::draw() const{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);
}


Base3D::~Base3D(){
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

Sphere::Sphere(std::array<float, 3>&& c, float r, unsigned int res)
    : center(std::move(c)), radius(r), resolution(res) {

        generateGeometry();
        try{
            setupBuffers();
        }catch(const std::invalid_argument& e){
            std::cerr << e.what() << std::endl;
        }
}

// //override
// void Sphere::draw() const{}


// override
void Sphere::generateGeometry() { 
    // This returns vertices and indices arrays (as one list) for a sphere having center and radius given.
    // resoultion is the number of splits taken in the angle in both directions

    const float PI = 3.14159265359f;

    unsigned int stacks  = resolution;
    unsigned int sectors = resolution;

    float stackStep  = PI / stacks;
    float sectorStep = 2 * PI / sectors;

    // ---------- Generate vertices ----------
    for (unsigned int i = 0; i <= stacks; ++i)
    {
        float stackAngle = PI / 2 - i * stackStep; // from +pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);
        float z  = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * sectorStep;

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertices.push_back(x + center[0]);
            vertices.push_back(y + center[1]);
            vertices.push_back(z + center[2]);
        }
    }

    // ---------- Generate indices ----------
    for (unsigned int i = 0; i < stacks; ++i)
    {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // top stack
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // bottom stack
            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

}