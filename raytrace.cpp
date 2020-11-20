#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
using namespace std;

GLFWwindow* setup() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWwindow* window = glfwCreateWindow(glfwGetVideoMode(monitor)->width, glfwGetVideoMode(monitor)->height, "OpenGL", monitor, nullptr);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glfwInit();

    return window;
}

void render(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void reportShaderCompErr(GLuint shader) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        char err[512];
        glGetShaderInfoLog(shader, 512, NULL, err);

        printf(err);

        exit(0);
    }
}

GLuint createShader(char* filePath, GLenum shaderType) {
    // Load shader source into buffer
    ifstream file(filePath, ios::binary | ios::ate);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    char* source;
    file.read(source, size);

    // Assign source to shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    reportShaderCompErr(shader);

    return shader;
}

int main() {
    GLFWwindow* window = setup();

    // Resource allocation
    float vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Load shaders
    GLuint vertexShader = createShader("vert.glsl", GL_VERTEX_SHADER);
    GLuint fragShader = createShader("frag.glsl", GL_FRAGMENT_SHADER);

    render(window);

    // Resource freeing

    glfwTerminate();
}