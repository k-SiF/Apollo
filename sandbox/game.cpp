#include "game.h"
#include <iostream>

Game::Game() {}

void Game::start() {
    m_player = m_scene.addEntity<Player>(&m_quad);
    m_player->setScale(glm::vec2(0.2f, 0.2f));

    setBackground(0.1f, 0.1f, 0.15f);
}

void Game::update() {
    
}