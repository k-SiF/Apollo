#pragma once
#include <string>

struct ma_engine;
struct ma_sound;

namespace apollo {
    class Audio {
        public:
            Audio();
            ~Audio();
            
            Audio(const Audio&) = delete;
            Audio& operator=(const Audio&) = delete;

            void playSound(const std::string& path);
            void playMusic(const std::string& path, bool loop = true);
            void stopMusic();

            void setMasterVolume(float volume);
        private:
            ma_engine* m_engine = nullptr;
            ma_sound* m_music = nullptr;
            bool m_musicActive = false;
    };
}