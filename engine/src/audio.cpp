#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <apollo/audio.h>
#include <iostream>

namespace apollo {
    Audio::Audio() {
        m_engine = new ma_engine();
        if (ma_engine_init(nullptr, m_engine) != MA_SUCCESS) {
            std::cout << "Failed to initialise audio engine." << std::endl;
            delete m_engine;
            m_engine = nullptr;
            return;
        }
    }

    Audio::~Audio() {
        if (m_music) {
            if (m_musicActive) ma_sound_uninit(m_music);
            delete m_music;
            m_music = nullptr;
        }

        if (m_engine) {
            ma_engine_uninit(m_engine);
            delete m_engine;
            m_engine = nullptr;
        }
    }

    void Audio::playSound(const std::string& path) {
        if (!m_engine) return;
        ma_engine_play_sound(m_engine, path.c_str(), nullptr);
    }

    void Audio::playMusic(const std::string& path, bool loop) {
        if (!m_engine) return;

        stopMusic();

        m_music = new ma_sound();

        ma_result r = ma_sound_init_from_file(m_engine, path.c_str(), MA_SOUND_FLAG_STREAM, nullptr, nullptr, m_music);

        if (r != MA_SUCCESS) {
            std::cout << "Failed to load music!" << std::endl;
            delete m_music;
            m_music = nullptr;
            return;
        }

        ma_sound_set_looping(m_music, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start(m_music);
        m_musicActive = true;
    }

    void Audio::stopMusic() {
        if (m_music && m_musicActive) {
            ma_sound_uninit(m_music);
            delete m_music;
            m_music = nullptr;
            m_musicActive = false;
        }
    }

    void Audio::setMasterVolume(float volume) {
        if (m_engine) ma_engine_set_volume(m_engine, volume);
    }
}