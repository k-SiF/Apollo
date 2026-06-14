#pragma once
#include <apollo/entity.h>

class Player : public Entity {
    public:
        Player(Mesh* mesh, glm::vec2 position = glm::vec2(0.0f));
        void update(float deltaTime) override;
    
    private:
        void handleInput(float deltaTime);
};