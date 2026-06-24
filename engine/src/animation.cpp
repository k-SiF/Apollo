#include <apollo/animation.h>

namespace apollo {
    Animation::Animation(std::vector<Frame> frames, float frameTime, bool loop) 
        : m_frames(std::move(frames)), m_frameTime(frameTime), m_loop(loop){

    }

    void Animation::update(float deltaTime) {
        if (m_frames.size() <= 1 || m_finished) return;

        m_timer += deltaTime;
        while (m_timer >= m_frameTime) {
            m_timer -= m_frameTime;
            m_current++;

            if (m_current >= m_frames.size()) {
                if (m_loop) {
                    m_current = 0;
                } else {
                    m_current = m_frames.size() - 1;
                    m_finished = true;
                    break;
                }
            }

        }
    }

    const Frame& Animation::currentFrame() const {
        return m_frames[m_current];
    }

    void Animation::reset() {
        m_current = 0;
        m_timer = 0.0f;
        m_finished = false;
    }

    // Helper functions

    std::vector<Frame> framesFromTextures(const std::vector<Texture*>& textures) {
        std::vector<Frame> frames;
        frames.reserve(textures.size());
        for (Texture* t : textures) {
            frames.push_back({t, UVRect{}}); // UVRect{} = full texture
        }
        return frames;
    }

    std::vector<Frame> framesFromSheet(Texture* sheet, int cols, int rows, int count) {
        std::vector<Frame> frames;
        if (cols <= 0 || rows <= 0) return frames;

        float cw = 1.0f / static_cast<float>(cols); // cell width (UV)
        float ch = 1.0f / static_cast<float>(rows); // cell height (UV)

        int total = (count < 0) ? cols * rows : count;
        frames.reserve(total);

        for (int i = 0; i < total; i++) {
            int col = i % cols;
            int row = i / cols;
            frames.push_back({sheet, UVRect{col * cw, row * ch, cw, ch}});
        }
        
        return frames;
    }
}