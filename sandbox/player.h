#pragma once
#include <apollo/entity.h>
#include <apollo/texture.h>

using namespace apollo;

class Player : public Entity {
    public:
        Player(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
        void update() override;
    
    private:
        void handleInput(float deltaTime);

        float m_timer = 0.0f;
        float m_velocityY = 0.0f;
        const float JUMP_FORCE = 5.0f;
        const float G = -9.81;

        bool m_jumping = false;

        Texture tex{"assets/Cardicon.png"};
};