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

        m_sfxGroup = new ma_sound();
        m_musicGroup = new ma_sound();
        ma_sound_group_init(m_engine, 0, nullptr, m_sfxGroup);
        ma_sound_group_init(m_engine, 0, nullptr, m_musicGroup);

    }

    Audio::~Audio() {
        if (m_music) {
            if (m_musicActive) ma_sound_uninit(m_music);
            delete m_music;
            m_music = nullptr;
        }
        if (m_sfxGroup) {
            ma_sound_group_uninit(m_sfxGroup);
            delete m_sfxGroup;
            m_sfxGroup = nullptr;
        }
        if (m_musicGroup) {
            ma_sound_group_uninit(m_musicGroup);
            delete m_musicGroup;
            m_musicGroup = nullptr;
        }
        if (m_engine) {
            ma_engine_uninit(m_engine);
            delete m_engine;
            m_engine = nullptr;
        }
    }

    void Audio::playSound(const std::string& path) {
        if (!m_engine) return;
        ma_engine_play_sound_ex(m_engine, path.c_str(), (ma_node*)m_sfxGroup, 0);
    }

    void Audio::playMusic(const std::string& path, bool loop) {
        if (!m_engine) return;
        stopMusic();

        m_music = new ma_sound();

        ma_result r = ma_sound_init_from_file(m_engine, path.c_str(), MA_SOUND_FLAG_STREAM, m_musicGroup, nullptr, m_music);
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

    void Audio::setSfxVolume(float volume) {
        if (m_sfxGroup) ma_sound_group_set_volume(m_sfxGroup, volume);
    }
 
    void Audio::setMusicVolume(float volume) {
        if (m_musicGroup) ma_sound_group_set_volume(m_musicGroup, volume);
    }

}