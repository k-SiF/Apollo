#pragma once

namespace apollo {
    enum class Key {
        #define APOLLO_KEY(name, glfwKey) name,
        #include <apollo/keys.def>
        #undef APOLLO_KEY
        Unknown,
        Count
    };
}