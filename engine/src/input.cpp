#include <apollo/input.h>

bool Input::s_current[10] = { false };
bool Input::s_previous[10] = { false };

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
    for (int i = 0; i < 10; i++) {
        s_previous[i] = s_current[i];
    }
}

void Input::setKeyState(Key key, bool pressed) {
    if (key == Key::Unknown) return;
    s_current[static_cast<int>(key)] = pressed;
}