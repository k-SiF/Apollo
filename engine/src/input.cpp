#include <apollo/input.h>

namespace apollo {
    bool Input::s_current[static_cast<int>(Key::Count)] = { false };
    bool Input::s_previous[static_cast<int>(Key::Count)] = { false };

    bool Input::onKeyPress(Key key) {
        return s_current[static_cast<int>(key)];
    }

    bool Input::onKeyRelease(Key key) {
        int i = static_cast<int>(key);
        return !s_current[i] && s_previous[i];
    }

    bool Input::wasKeyPressed(Key key) {
        int i = static_cast<int>(key);
        return s_current[i] && !s_previous[i];
    }

    void Input::update() {
        for (int i = 0; i < static_cast<int>(Key::Count); i++) {
            s_previous[i] = s_current[i];
        }
    }

    void Input::setKeyState(Key key, bool pressed) {
        if (key == Key::Unknown) return;
        s_current[static_cast<int>(key)] = pressed;
    }
}