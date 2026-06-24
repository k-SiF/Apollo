#include <apollo/resources.h>
#include <apollo/image.h>
#include <future>
#include <vector>

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

    void Resources::preload(const std::vector<std::string>& paths) {
        // Decode in parallel 
        std::vector<std::future<DecodedImage>> futures;
        for (const std::string& path : paths) {
            if (m_textures.count(path)) continue; // already loaded
            futures.push_back(std::async(std::launch::async,
                                         DecodedImage::decode, path));
        }
 
        // Upload on this thread
        for (auto& fut : futures) {
            DecodedImage img = fut.get();
            if (m_textures.count(img.path)) { img.free(); continue; }  // race guard
            m_textures[img.path] = std::make_unique<Texture>(img);     // GPU upload
            img.free();                                                // free CPU pixels
        }
    }
}