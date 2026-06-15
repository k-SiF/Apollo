#include "mesh.h"

#include <glad/gl.h>

namespace apollo {
    Mesh::Mesh(Mesh&& other) noexcept 
        : m_vao(other.m_vao), m_vbo(other.m_vbo), m_vertexCount(other.m_vertexCount) {
            other.m_vao = 0;
            other.m_vbo = 0;
            other.m_vertexCount = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &m_vao);
            glDeleteBuffers(1, &m_vbo);
            m_vao = other.m_vao; m_vbo = other.m_vbo; m_vertexCount = other.m_vertexCount;
            other.m_vao = 0; other.m_vbo = 0; other.m_vertexCount = 0;
        }
        return *this;
    }

    Mesh::Mesh(const std::vector<float>& vertices) {
        m_vertexCount = static_cast<int>(vertices.size() / 3);

        // Upload vertex data to GPU(VBO)
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Data layout (VAO)
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void Mesh::draw() const {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    }

    Mesh Mesh::createTriangle() {
        return Mesh({ -0.5f, -0.5f, 0.0f,
                    0.5f, -0.5f, 0.0f,
                    0.0f,  0.5f, 0.0f });
    }

    Mesh Mesh::createQuad() {
        return Mesh({
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        });
    }
}