#pragma once
#include <vector>

namespace apollo {
    class Mesh {
        public:
            Mesh(Mesh&& other) noexcept;            // move constructor
            Mesh& operator=(Mesh&& other) noexcept; // move assignment
            Mesh(const std::vector<float>& vertices);
            ~Mesh();
            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;

            void draw() const;
            static Mesh createTriangle();
            static Mesh createQuad();

        private:
            unsigned int m_vbo = 0;
            unsigned int m_vao = 0;
            int m_vertexCount = 0;
    };
}