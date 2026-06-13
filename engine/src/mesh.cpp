#include "mesh.h"

#include <glad/gl.h>

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