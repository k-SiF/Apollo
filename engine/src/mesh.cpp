#include <apollo/mesh.h>

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
        m_vertexCount = static_cast<int>(vertices.size() / 5);

        // Upload vertex data to GPU(VBO)
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Data layout (VAO)
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // Position attribute (location 0): 3 floats, starting at offset 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // UV attribute (location 1): 2 floats, starting after the 3 position floats
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
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
            // positions (x, y, z) // UVs
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // bottom-left
             0.5f, -0.5f, 0.0f,    1.0f, 0.0f,   // bottom-right
             0.5f,  0.5f, 0.0f,    1.0f, 1.0f,   // top-right
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // bottom-left
             0.5f,  0.5f, 0.0f,    1.0f, 1.0f,   // top-right
            -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // top-left
        });
    }
}