#pragma once
#include <apollo/texture.h>
#include <apollo/uvrect.h> 
#include <vector>

namespace apollo {
    struct Frame {
        Texture* texture = nullptr;
        UVRect uv;
    };

    class Animation {
        public:
            Animation() = default;
            Animation(std::vector<Frame> frames, float frameTime, bool loop = true);

            void update(float deltaTime);
            const Frame& currentFrame() const;
            void reset();
            bool finished() const { return m_finished; }
            bool valid() const { return !m_frames.empty(); }
        private:
            std::vector<Frame> m_frames;
            float m_frameTime = 0.1f;
            float m_timer = 0.0f;
            size_t m_current = 0;
            bool m_loop = true;
            bool m_finished = false;


    };

    std::vector<Frame> framesFromTextures(const std::vector<Texture*>& textures);

    std::vector<Frame> framesFromSheet(Texture* sheet, int cols, int rows, int count = -1);
}