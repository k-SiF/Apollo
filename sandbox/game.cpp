#include "game.h"
#include <iostream>


Game::Game() {}

void Game::start() {
    m_player = m_scene.addEntity<Player>(&m_quad);
    m_player->setPosition(glm::vec2(1.5f));

    setBackground(0.1f, 0.1f, 0.15f);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            marker = m_scene.addEntity<Entity>(&m_quad);
            marker->setPosition(glm::vec2(x, y));
            marker->setScale(glm::vec2(0.1f));
            marker->setTexture(&m_grassTexture);
        }
    }
}

void Game::update(float deltaTime) {
    if (Input::wasKeyPressed(Key::L)) m_renderer.m_debugDraw = (m_renderer.m_debugDraw) ? false : true;
    if (Input::wasKeyPressed(Key::F11)) toggleFullscreen();
}

void Game::onRender(float alpha) {
    glm::vec2 pos = m_player->getRenderPosition(alpha);
    glm::vec2 curr = m_camera.getPosition();
    m_camera.setPosition(curr + (pos - curr) * 0.1f);
}