#include <apollo/resources.h>

namespace apollo {
    Texture* Resources::getTexture(const std::string& path) {
        // loaded
        auto t = m_textures.find(path);
        if (t != m_textures.end()) {
            return t->second.get();
        }

        // not loaded
        auto texture = std::make_unique<Texture>(path);
        Texture* ptr = texture.get();
        m_textures[path] = std::move(texture);
        return ptr;
    }
}