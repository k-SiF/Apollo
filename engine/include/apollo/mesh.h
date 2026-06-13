#pragma once
#include <vector>

class Mesh {
    public:
        Mesh(const std::vector<float>& vertices);
        ~Mesh();
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void draw() const;

    private:
        unsigned int m_vbo = 0;
        unsigned int m_vao = 0;
        int m_vertexCount = 0;
};