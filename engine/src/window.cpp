#include "window.h"

#include<iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <apollo/key.h>
#include <apollo/input.h>

static Key glfwToApolloKey(int glfwKey) {
    switch (glfwKey) {
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_UP:    return Key::Up;
        case GLFW_KEY_DOWN:  return Key::Down;
        case GLFW_KEY_LEFT:  return Key::Left;
        case GLFW_KEY_RIGHT: return Key::Right;
        case GLFW_KEY_SPACE:  return Key::Space;
        case GLFW_KEY_ESCAPE: return Key::Esc;
        default: return Key::Unknown;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    bool pressed = (action != GLFW_RELEASE);
    Input::setKeyState(glfwToApolloKey(key), pressed);
}

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
    glfwSetKeyCallback(m_window, keyCallback);
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
    Input::update();
    glfwPollEvents();
}

int Window::getWidth() const { 
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return w; 
}

int Window::getHeight() const { 
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    return h; 
}

float Window::getTime() const {
    return static_cast<float>(glfwGetTime());
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

