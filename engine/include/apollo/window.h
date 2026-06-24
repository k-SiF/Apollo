#pragma once
struct GLFWwindow;

namespace apollo {
    enum WindowMode {
        Windowed,
        Fullscreen,
        Borderless
    };

    struct WindowConfig {
        int width = 800;
        int height = 600;
        const char* title = "Apollo";
        WindowMode mode = WindowMode::Windowed;   // start mode
        bool vsync = true;
        int maxFPS = 0; // 0 = uncapped
    };

    class Window {
        public:
            Window(const WindowConfig& config);
            ~Window();
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            void setMode(WindowMode mode);
            bool isFullscreen() const { return m_fullscreen; }
            void toggleFullscreen();
            void applyVSync() const;

            bool shouldClose() const;
            void close() const;
            void swapBuffers();
            void pollEvents();
            void clear(float r, float g, float b, float a = 1.0f);
            double getTime() const;
            GLFWwindow* handle() const { return m_window; }

            int getWidth() const;
            int getHeight() const;

        private:
            GLFWwindow* m_window = nullptr;
            WindowMode m_mode = WindowMode::Windowed;
            int m_windowedWidth = 800;
            int m_windowedHeight = 600;
            int m_windowedX = 100, m_windowedY = 100;
            bool m_fullscreen = false;
    };
}