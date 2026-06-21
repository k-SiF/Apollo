#pragma once
struct GLFWwindow;

namespace apollo {
    class Window {
        public:
            Window(int width, int height, const char* title);
            ~Window();
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            void setFullscreen(bool fullscreen);
            bool isFullscreen() const { return m_fullscreen; }
            void toggleFullscreen() { setFullscreen(!m_fullscreen); }
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
            int m_windowedWidth = 800;
            int m_windowedHeight = 600;
            int m_windowedX = 100, m_windowedY = 100;
            bool m_fullscreen = false;
    };
}