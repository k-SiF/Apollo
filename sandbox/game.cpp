#include "game.h"
#include <iostream>


Game::Game() : Sketch(WindowConfig{ 1280, 720, "Liya's Adventure", WindowMode::Windowed, true, 0}) {}

static std::vector<Frame> loadFrames(Resources& res, const std::string& folder,
                                     const std::string& prefix, int count) {
    std::vector<Texture*> textures;
    for (int i = 1; i <= count; i++) {
        std::string path = folder + "/" + prefix + std::to_string(i) + ".png";
        textures.push_back(res.getTexture(path));
    }
    return framesFromTextures(textures);
}

void Game::start() {
    Texture* liya = m_resources.getTexture("assets/idle/Liya idle_1.png");
    Texture* grass = m_resources.getTexture("assets/grass_palette.png");
    Texture* campfire_sheet = m_resources.getTexture("assets/campfire_sheet.png", TextureFilter::Nearest);
    m_player = m_scene.addEntity<Player>(&m_quad, liya);
    m_player->setSortLayer(0);
    m_player->setOrderInLayer(2);
    m_player->anchorBottom();
    m_player->setPosition(glm::vec2(1.5f));

    std::vector<std::string> allFrames;
    for (int i = 1; i <= 10; i++)
        allFrames.push_back("assets/idle/Liya idle_" + std::to_string(i) + ".png");
    for (int i = 1; i <= 8; i++)
        allFrames.push_back("assets/run/Liya run_" + std::to_string(i) + ".png");
    m_resources.preload(allFrames);

    m_player->addAnimation("idle",
        Animation(loadFrames(m_resources, "assets/idle", "Liya idle_", 10), 0.1f), 1.0f);
    m_player->addAnimation("run",
        Animation(loadFrames(m_resources, "assets/run",  "Liya run_",   8), 0.08f), 0.48f);
    m_player->addAnimation("effect", Animation(framesFromSheet(campfire_sheet, 5, 8), 0.08f));

    setBackground(0.1f, 0.1f, 0.15f);

    for (int x = 0; x <= 1; x++) {
        for (int y = 0; y <= 1; y++) {
            marker = m_scene.addEntity<Entity>(&m_quad);
            marker->setPosition(glm::vec2(x, y));
            marker->setScale(glm::vec2(0.1f));
            marker->setTexture(grass);
        }
    }

    campfire = m_scene.addEntity<Entity>(&m_quad);
    campfire->setSortLayer(0);
    campfire->setOrderInLayer(1);
    campfire->setCollidable(false);
    campfire->anchorBottom();
    campfire->setPosition(glm::vec2(0.0f, -0.5f));
    campfire->addAnimation("effect", Animation(framesFromSheet(campfire_sheet, 5, 8), 0.08f), 0.2f);
    campfire->play("effect");


    m_audio.playMusic("assets/music/Littleroot Town.mp3");
    m_audio.setMasterVolume(0.7f);
    m_audio.setMusicVolume(0.4f);
    m_audio.setSfxVolume(0.6f);

    m_player->setAudio(&m_audio);

    setMode(WindowMode::Windowed);
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

void Game::drawWorld(float alpha) {
    const float groundY = -0.5f;
    m_renderer.drawDebugLine(glm::vec2(-10.0f, groundY),
                             glm::vec2( 10.0f, groundY));
}

void Game::drawUI() {
    // screen-space text — (x, y) in pixels from the top-left
    m_renderer.drawText(m_font, "Apollo Engine", 20.0f, 20.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    m_renderer.drawText(m_font, "Score: 0", 20.0f, 70.0f, glm::vec3(1.0f, 0.9f, 0.2f));
}
