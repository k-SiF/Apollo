#include <string>

class Shader {
    public:
        Shader(const char* filepath);
        ~Shader();
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use() const;
        unsigned int compile(unsigned int type, const std::string& source);


    private:
        unsigned int m_id = 0;
};