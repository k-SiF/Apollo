#include "game.h"
#include <iostream>

Game::Game() {}

void Game::start() {
    m_player = m_scene.addEntity<Player>(&m_quad);
    m_player->setScale(glm::vec2(0.2f, 0.2f)); 

    setBackground(0.1f, 0.1f, 0.15f);

    for (int x = -3; x <= 3; x++) {
        for (int y = -3; y <= 3; y++) {
            Entity* marker = m_scene.addEntity<Entity>(&m_quad);
            marker->setPosition(glm::vec2(x, y));
            marker->setScale(glm::vec2(0.1f));
            marker->setTexture(&m_grassTexture);  // or any texture
        }
    }
}

void Game::update() {
    m_camera.setPosition(m_player->getPosition());
}