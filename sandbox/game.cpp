#include "game.h"

Game::Game() {}

void Game::start() {
    m_player = m_scene.addEntity<Player>(&m_quad);

    setBackground(0.1f, 0.1f, 0.15f);
}

void Game::update(float deltaTime) {

}