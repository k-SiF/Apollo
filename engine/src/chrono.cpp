#include "chrono.h"

namespace apollo {
    float Time::s_deltaTime = 0.0f;
    float Time::s_elapsed = 0.0f;

    float Time::deltaTime() { return s_deltaTime; }
    float Time::elapsed() { return s_elapsed; }

    void Time::update(float deltaTime) {
        s_deltaTime = deltaTime;
        s_elapsed += deltaTime;
    }
}