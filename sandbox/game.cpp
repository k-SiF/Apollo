#include "game.h"
#include <iostream>


Game::Game() {}

void Game::start() {
    Texture* liya = m_resources.getTexture("assets/Liya_idle_1.png");
    Texture* grass = m_resources.getTexture("assets/grass_palette.png");
    m_player = m_scene.addEntity<Player>(&m_quad, liya);
    m_player->setPosition(glm::vec2(1.5f));

    setBackground(0.1f, 0.1f, 0.15f);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            marker = m_scene.addEntity<Entity>(&m_quad);
            marker->setPosition(glm::vec2(x, y));
            marker->setScale(glm::vec2(0.1f));
            marker->setTexture(grass);
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