#include "Objects.h"
#include <stdexcept>


void RenderObj::updateModel(glm::mat4 mod){
    model = mod;
}

void RenderObj::updateColor(glm::vec3 col){
    color = col;
}

void Base3D::setupBuffers(){
    // check vertices and indices validity - raise error if not
    if (vertices.empty() or indices.empty()){
        std::cerr << "Requested buffer init with empty data vectors!";
        throw;
    }

    // check vao is already set - return early if so
    if (VAO != 0) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind & fill
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glm::vec3* verts = vertices.data();
    unsigned int* inds = indices.data();

    // note to self - better way to take the size than vertices.size() * sizeof(float) ??? - issue - dtype hardcoded
    glBufferData(GL_ARRAY_BUFFER,  vertices.size() * sizeof(glm::vec3), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), inds, GL_STATIC_DRAW);

    // attribute 0: position (vec3) -> offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Locus::setupBuffers(){

    // check vao is already set - return early if so
    if (VAO != 0) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_TRAJECTORY_POINTS * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Base3D::draw(GLint locModel, GLint colorLoc) const{
    if (VAO == 0) {
        std::cerr << "Warning: Attempting to draw uninitialized object (Base3d)\n";
        throw;
    }
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
    // glUniform3f(colorLoc, 1.0f, 1.0f, 0.2f);
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);
}

void Locus::draw(GLint locModel, GLint colorLoc) const{
    if (VAO == 0) {
        std::cerr << "Warning: Attempting to draw uninitialized object (Locus)\n";
        throw;
    }
    if (vertices.size() > 1) {
            glBindBuffer(GL_COPY_WRITE_BUFFER, VBO);
            glBufferSubData(GL_COPY_WRITE_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
            
            glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
            // glUniform3f(colorLoc, 0.5f, 0.8f, 0.2f);  // greenish
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));
            
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)vertices.size());
            glBindVertexArray(0);
    }
}

void Locus::updateLocus(glm::vec3& interpolated_position){
    if (vertices.empty() || glm::distance(vertices.back(), interpolated_position) > 0.05f) {
				vertices.push_back(interpolated_position);
				if (vertices.size() > MAX_TRAJECTORY_POINTS) {
					vertices.erase(vertices.begin());
				}
			}
}

Base3D::~Base3D(){
    // if (VAO) glDeleteBuffers(1, &VBO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    std::cout << "Base3D buffer deleted successfully\n";
}

Locus::~Locus(){
    // if (VAO) glDeleteBuffers(1, &VBO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
    std::cout << "Locus buffer deleted successfully\n";

}

Sphere::Sphere(float r, unsigned int res, glm::vec3&& c, glm::vec3&& col)
    : Base3D(), center(std::move(c)), radius(r), resolution(res) {

        if (resolution == 0) {
            throw std::invalid_argument("Resolution must be > 0");
        }

        color = std::move(col);
        generateGeometry();
        setupBuffers();
        
}

Locus::Locus() {
        VAO = 0;
        VBO = 0;
        color = glm::vec3(0.5f, 0.8f, 0.2f);
        model = glm::mat4(1.0f);
        setupBuffers();
    }

// //override
// void Sphere::draw() const{}


// override
void Sphere::generateGeometry() { 
    // This returns vertices and indices arrays (as one list) for a sphere having center and radius given.
    // resoultion is the number of splits taken in the angle in both directions

    if (resolution == 0) {
        throw std::invalid_argument("Resolution must be > 0");
    }

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

            vertices.push_back(glm::vec3(x, y, z) + center);
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