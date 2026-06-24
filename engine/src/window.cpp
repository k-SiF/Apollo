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
            #define APOLLO_KEY(name, glfwKey) case glfwKey: return Key::name;
            #include <apollo/keys.def>
            #undef APOLLO_KEY
            
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

    void Window::setMode(WindowMode mode) {
        if (mode == m_mode) return;
 
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vid = glfwGetVideoMode(monitor);
 
        // If we're currently windowed, save geometry so we can restore it later.
        if (m_mode == WindowMode::Windowed) {
            glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
            glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
        }
 
        switch (mode) {
            case WindowMode::Windowed: {
                // restore decorations and the saved windowed geometry
                glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
                glfwSetWindowMonitor(m_window, nullptr,
                                     m_windowedX, m_windowedY,
                                     m_windowedWidth, m_windowedHeight, 0);
                break;
            }
 
            case WindowMode::Fullscreen: {
                // EXCLUSIVE fullscreen: pass the monitor. Bypasses compositor.
                // Smoothest, but screen-capture tools (Discord/OBS) can't grab it.
                glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
                glfwSetWindowMonitor(m_window, monitor, 0, 0,
                                     vid->width, vid->height, vid->refreshRate);
                break;
            }
 
            case WindowMode::Borderless: {
                // BORDERLESS fullscreen: a windowed window (NULL monitor) with no
                // decorations, sized to fill the screen. Still composited by DWM,
                // so it's STREAMABLE and alt-tab friendly. Pacing is compositor's.
                glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
                glfwSetWindowMonitor(m_window, nullptr, 0, 0,
                                     vid->width, vid->height, 0);
                break;
            }
        }
 
        m_mode = mode;
        applyVSync();
    }

    void Window::toggleFullscreen() {
        switch (m_mode) {
            case WindowMode::Windowed:   setMode(WindowMode::Fullscreen);  break;
            case WindowMode::Fullscreen: setMode(WindowMode::Borderless);  break;
            case WindowMode::Borderless: setMode(WindowMode::Windowed);    break;
        }
    }

    void Window::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
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
            if (m_mode == WindowMode::Fullscreen) {
                glfwSwapInterval(1);
            } else {
                glfwSwapInterval(0);
            }
        #else
            glfwSwapInterval(1);
        #endif
    }

    // ---- in swapBuffers(), the DwmFlush guard now covers BOTH windowed modes ----
    void Window::swapBuffers() {
        glfwSwapBuffers(m_window);
    #ifdef _WIN32
        if (m_mode != WindowMode::Fullscreen) {   // windowed OR borderless
            DwmFlush();
        }
    #endif
    }

}
