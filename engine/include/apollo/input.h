#pragma once
#include <apollo/key.h>

class Input {
    public:
        static bool onKeyPress(Key key);    // Key press right now.
        static bool onKeyRelease(Key key);  // Pressed last frame, released this frame.
        static bool wasKeyPressed(Key key); // Up last frame, pressed this frame. 
        
        Input() = delete; // No instances (Utility only)
        
        // Engine-internal functions
        static void update();
        static void setKeyState(Key key, bool pressed);
        
    private:
        static bool s_current[static_cast<int>(Key::Count)];
        static bool s_previous[static_cast<int>(Key::Count)];
};