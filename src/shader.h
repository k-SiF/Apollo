#include <string>

class Shader {
    public:
        Shader(const char* filepath);
        ~Shader();
        void use() const;
        unsigned int compile(unsigned int type, const std::string& source);

    private:
        unsigned int m_id = 0;
};