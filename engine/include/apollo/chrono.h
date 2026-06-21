#pragma once

namespace apollo {
    class Time {
        public:
            static float deltaTime(); // seconds since last frame.
            static float fixedTime();
            static float elapsed();   // seconds since the program started.
            
            Time() = delete;

            // engine-internal
            static void update(float deltaTime);
            static void fixedUpdate(float deltaTime);

        private:
            static float s_deltaTime;
            static float s_FIXEDTIME;
            static float s_elapsed;
    };
}