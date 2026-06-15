#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp> 

namespace apollo {
    class Shader {
        public:
            Shader(const char* filepath);
            ~Shader();
            Shader(const Shader&) = delete;
            Shader& operator=(const Shader&) = delete;

            void use() const;
            unsigned int compile(unsigned int type, const std::string& source);

            void setVec2(const char* name, float x, float y) const;
            void setMat4(const char* name, const glm::mat4& matrix) const;
            void setInt(const char* name, int value) const;


        private:
            unsigned int m_id = 0;
    };
}