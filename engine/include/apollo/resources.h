#pragma once
#include <apollo/texture.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace apollo {
    class Resources {
        public:
            Resources() = default;

            // Non-copyable
            Resources(const Resources&) = delete;
            Resources& operator=(const Resources&) = delete;

            Texture* getTexture(const std::string& path, TextureFilter filter = TextureFilter::Linear);
            
            void preload(const std::vector<std::string>& paths);

        private:
            std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
    };
}