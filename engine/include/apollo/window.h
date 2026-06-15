#pragma once
struct GLFWwindow;

namespace apollo {
    class Window {
        public:
            Window(int width, int height, const char* title);
            ~Window();
            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            bool shouldClose() const;
            void swapBuffers();
            void pollEvents();
            void clear(float r, float g, float b, float a = 1.0f);
            float getTime() const;
            GLFWwindow* handle() const { return m_window; }

            int getWidth() const;
            int getHeight() const;

        private:
            GLFWwindow* m_window = nullptr;
    };
}