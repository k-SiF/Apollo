#pragma once
#include <apollo/entity.h>
#include <apollo/texture.h>

using namespace apollo;

class Player : public Entity {
    public:
        Player(Mesh* mesh, Texture* texture, glm::vec2 position = glm::vec2(0.0f));
        void update(float deltaTime) override;
        void fixedUpdate(float deltaTime) override;
        void onCollision(Entity& other) override;

        bool m_canMove = true;
    
    private:
        void handleInput(float deltaTime);
        void jump(float y);
        void flip(glm::vec2& scale, int dir);

        float m_timer = 0.0f;
        const float SPEED = 0.5f;
        const float JUMP_FORCE = 5.0f;
        const float G = -9.81;
        
        bool m_jump = false;
        
        glm::vec2 m_velocity = glm::vec2(SPEED, JUMP_FORCE);
        glm::vec2 m_prevPos = glm::vec2(0.0f);

        Texture* m_texture = nullptr;
};