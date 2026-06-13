#include "shader.h"

#include <iostream>
#include <fstream>
#include <glad/gl.h>

Shader::Shader(const char* filepath) {
    std::ifstream File(filepath);

    if (!File.is_open()) {
        std::cout << "Failed to open Shader file: " << filepath << std::endl;
        return;
    }

    std::string line;
    bool vertexCheck = true;
    std::string vertexSource, fragmentSource;
    while (std::getline(File, line)) {
        if (line.find("#shader") == 0) {
            if (line.find("vertex") != std::string::npos) vertexCheck = true;
            if (line.find("fragment") != std::string::npos) vertexCheck = false;
        } else {
            (vertexCheck ? vertexSource : fragmentSource) += line + '\n';
        }
    }   
    File.close();

    unsigned int vertexShader = compile(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentSource);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    int success;
    glGetProgramiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(m_id, 521, nullptr, log);
        std::cout << "Program link error: " << log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {};

unsigned int Shader::compile(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cout << "Shader compile error ("
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "):" << log << std::endl;
    }

    return shader;
}

void Shader::use() const {
    if (m_id == 0) {
        static bool warned = false;
        if (!warned) {
            std::cout << "Shader::use() called on invalid shader!" << std::endl;
            warned = true;
        }
        return;
    }
    glUseProgram(m_id);
}

void Shader::setVec2(const char *name, float x, float y) const {
    int location = glGetUniformLocation(m_id, name);
    if (location == -1) {
        std::cout << "Uniform not found: " << name << std::endl;
        return;
    }
    glUniform2f(location, x, y);    
}

