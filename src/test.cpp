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

// ---------- Shader sources ----------
const char *vertexShaderSrc = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;       // vertex position
layout(location = 1) in vec3 aColor;     // per-vertex color

out vec3 vColor;                         // pass to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    // gl_Position expects clip-space coordinates. We use column-major matrices.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)glsl";

const char *fragmentShaderSrc = R"glsl(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0); // use interpolated color
}
)glsl";
// --------------------------------------
void showFPS(GLFWwindow* window) {
    static double previousTime = glfwGetTime();
    static int frameCount = 0;

    double currentTime = glfwGetTime();
    double delta = currentTime - previousTime;
    frameCount++;

    // Update every 1 second
    if (delta >= 1.0) {
        double fps = double(frameCount) / delta;
        std::string title = "OpenGL Window - FPS: " + std::to_string((int)fps);
        glfwSetWindowTitle(window, title.c_str());

        frameCount = 0;
        previousTime = currentTime;
    }
}

// ---------- Minimal math helpers (4x4 matrices in column-major order) ----------
struct Mat4 {
    float m[16];
    // identity
    static Mat4 identity() {
        Mat4 r{};
        for (int i=0;i<16;++i) r.m[i] = 0.0f;
        r.m[0]=r.m[5]=r.m[10]=r.m[15]=1.0f;
        return r;
    }
};

// multiply A * B (column-major)
Mat4 mul(const Mat4 &A, const Mat4 &B) {
    Mat4 R{};
    for (int row=0; row<4; ++row) {
        for (int col=0; col<4; ++col) {
            float s = 0.0f;
            for (int k=0; k<4; ++k) {
                // element (row,col) = sum_k A(row,k) * B(k,col)
                s += A.m[k*4 + row] * B.m[col*4 + k];
            }
            R.m[col*4 + row] = s;
        }
    }
    return R;
}

Mat4 translate(float x, float y, float z) {
    Mat4 r = Mat4::identity();
    r.m[12] = x;
    r.m[13] = y;
    r.m[14] = z;
    return r;
}

Mat4 rotateY(float radians) {
    Mat4 r = Mat4::identity();
    float c = cosf(radians), s = sinf(radians);
    // column-major indexing:
    // [  c  0  s  0 ]
    // [  0  1  0  0 ]
    // [ -s  0  c  0 ]
    // [  0  0  0  1 ]
    r.m[0] =  c; r.m[8]  = -s;
    r.m[2] =  s; r.m[10] =  c;
    return r;
}

Mat4 perspective(float fovYRadians, float aspect, float znear, float zfar) {
    float f = 1.0f / tanf(fovYRadians * 0.5f);
    Mat4 r{};
    for (int i=0;i<16;++i) r.m[i]=0.0f;
    r.m[0] = f / aspect;
    r.m[5] = f;
    r.m[10] = (zfar + znear) / (znear - zfar);
    r.m[11] = -1.0f;
    r.m[14] = (2.0f * zfar * znear) / (znear - zfar);
    return r;
}

Mat4 lookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ) {
    // compute forward, right, up vectors
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;
    // normalize f
    float flen = sqrtf(fx*fx + fy*fy + fz*fz);
    fx /= flen; fy /= flen; fz /= flen;
    // up normalized
    float ux = upX, uy = upY, uz = upZ;
    float ulen = sqrtf(ux*ux + uy*uy + uz*uz);
    ux /= ulen; uy /= ulen; uz /= ulen;
    // s = f x up
    float sx = fy * uz - fz * uy;
    float sy = fz * ux - fx * uz;
    float sz = fx * uy - fy * ux;
    float slen = sqrtf(sx*sx + sy*sy + sz*sz);
    sx /= slen; sy /= slen; sz /= slen;
    // u' = s x f
    float ux2 = sy * fz - sz * fy;
    float uy2 = sz * fx - sx * fz;
    float uz2 = sx * fy - sy * fx;

    Mat4 M = Mat4::identity();
    // column-major placement:
    M.m[0] = sx; M.m[4] = ux2; M.m[8]  = -fx; M.m[12] = 0.0f;
    M.m[1] = sy; M.m[5] = uy2; M.m[9]  = -fy; M.m[13] = 0.0f;
    M.m[2] = sz; M.m[6] = uz2; M.m[10] = -fz; M.m[14] = 0.0f;
    M.m[3] = 0;  M.m[7] = 0;    M.m[11] = 0;   M.m[15] = 1.0f;

    Mat4 T = translate(-eyeX, -eyeY, -eyeZ);
    return mul(M, T);
}
// ---------------------------------------------------------------------------

// ---------- Utility: compile & link shaders ----------
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GLuint createProgram(const char* vsrc, const char* fsrc) {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsrc);
    if (!vs || !fs) return 0;
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    int success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(prog, 512, nullptr, infoLog);
        std::cerr << "Program link error:\n" << infoLog << std::endl;
        return 0;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}
// ---------------------------------------------------------------------------

int main() {
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

    GLuint program = createProgram(vertexShaderSrc, fragmentShaderSrc);
    if (!program) {
        std::cerr << "Failed to create shader program\n";
        return -1;
    }

    // get uniform locations
    GLint locModel = glGetUniformLocation(program, "model");
    GLint locView  = glGetUniformLocation(program, "view");
    GLint locProj  = glGetUniformLocation(program, "projection");

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // clear color + depth
        glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // compute simple transforms
        float time = (float)glfwGetTime();
        // Mat4 model = rotateY(time * 0.8f);              // spin over Y
        // Mat4 view  = lookAt(1.5f, 1.0f, 2.0f,            // eye pos
        //                     0.0f, 0.0f, 0.0f,            // look at origin
        //                     0.0f, 1.0f, 0.0f);           // up
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (width > 0 && height > 0) ? (float)width / (float)height : 4.0f/3.0f;
        // Mat4 proj = perspective(45.0f * (3.1415926535f / 180.0f), aspect, 0.1f, 100.0f);

        // draw
        glUseProgram(program);
        // send matrices (column-major order); 1 = count; GL_FALSE = do not transpose
        // Create transformation matrices
        float timeValue = glfwGetTime();
        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
                                    time * glm::radians(20.0f),
                                    glm::vec3(0.5f, 1.0f, 0.0f));
        // glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
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
    glDeleteProgram(program);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
