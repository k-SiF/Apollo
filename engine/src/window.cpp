#ifdef _WIN32
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

#include <apollo/window.h>
#include <apollo/key.h>
#include <apollo/input.h>

#include<iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace apollo {
    static Key glfwToApolloKey(int glfwKey) {
        switch (glfwKey) {
            case GLFW_KEY_W: return Key::W;
            case GLFW_KEY_A: return Key::A;
            case GLFW_KEY_S: return Key::S;
            case GLFW_KEY_D: return Key::D;
            case GLFW_KEY_L: return Key::L;
            case GLFW_KEY_F11: return Key::F11;
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
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int fbW, fbH;
        glfwGetFramebufferSize(m_window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);

        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetKeyCallback(m_window, keyCallback);

        m_windowedWidth = width;
        m_windowedHeight = height;

        applyVSync();
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::setFullscreen(bool fullscreen) {
        if (fullscreen && !m_fullscreen) {
            // save current windowed pos/size before going fullscreen
            glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
            glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_window, monitor, 0, 0,
                                mode->width, mode->height, mode->refreshRate);
        }
        else if (!fullscreen && m_fullscreen) {
            // restore windowed pos/size
            glfwSetWindowMonitor(m_window, NULL,
                                m_windowedX, m_windowedY,
                                m_windowedWidth, m_windowedHeight, 0);
        }
        m_fullscreen = fullscreen;
        applyVSync();
    }

    void Window::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_window);

        #ifdef _WIN32
            if (!m_fullscreen) {
                DwmFlush();   // sync to compositor in windowed mode
            }
        #endif
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

    double Window::getTime() const {
        return glfwGetTime();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    void Window::close() const {
        glfwSetWindowShouldClose(m_window, true);
    }

    void Window::applyVSync() const {
        #ifdef _WIN32
            glfwSwapInterval(m_fullscreen ? 1 : 0);
        #else
            glfwSwapInterval(1);
        #endif
    }
}
