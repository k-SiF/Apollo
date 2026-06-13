#include "window.h"

#include<iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}


void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

float Window::getTime() const {
    return static_cast<float>(glfwGetTime());
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

