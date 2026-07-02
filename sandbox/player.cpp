#include "player.h"
#include <apollo/chrono.h>
#include <apollo/scene.h>
#include <apollo/input.h>
#include <apollo/key.h>
#include <iostream>

Player::Player(Mesh* mesh, Texture* texture, glm::vec2 position) : Entity(mesh, position), m_texture(texture) {
    setTextureSize(m_texture, 0.7f);
    setTexture(m_texture);

    setColSize(glm::vec2(0.12f, 0.3f));
}

void Player::update(float deltaTime) {
    if (Input::wasKeyPressed(Key::Space))  m_jump = true;

    if (m_moving) play("run");
    else play("idle");

    if (m_currentAnim) {
        m_currentAnim->update(deltaTime);
        const Frame& f = m_currentAnim->currentFrame();
        setTextureSize(f.texture, 0.7f * m_currentScale);
        glm::vec2 s = getScale();
        s.x = std::abs(s.x) * m_facing;
        setScale(s);

        setUVRect(f.uv);
    }
}

void Player::fixedUpdate(float deltaTime) {
    handleInput(deltaTime);

    // DEBUG MOVEMENT
    // glm::vec2 pos = getPosition();
    // pos.x += 0.5f * deltaTime;          // slower, easier to watch
    // if (pos.x > 1.0f) pos.x = -1.0f;    // wrap within visible range (-1 to 1)
    // pos.y = 0.0f;                        // keep it vertically centered
    // setPosition(pos);
}

void Player::onCollision(Entity& other) {
    
}

void Player::handleInput(float deltaTime) {
    glm::vec2 pos = getPosition();
    float r = getRotation();

    // --- horizontal input ---
    float moveX = 0.0f;
    m_moving = false;
    if (Input::onKeyPress(Key::Left))  { moveX = -SPEED;  m_facing = -1; m_moving = true; }
    if (Input::onKeyPress(Key::Right)) { moveX =  SPEED;  m_facing = 1; m_moving = true; }

    // --- jump ---
    if (m_jump) {
        jump(pos.y);
        m_jump = false;
    }
    m_velocity.y += G * deltaTime; 

    // --- X axis: check for valid move ---
    glm::vec2 tryX = glm::vec2(pos.x + moveX * deltaTime, pos.y);
    if (!getScene() || !getScene()->wouldCollide(getBoundsAt(tryX), this)) {
        pos.x = tryX.x;
    }

    // --- Y axis: check for valid move ---
    glm::vec2 tryY = glm::vec2(pos.x, pos.y + m_velocity.y * deltaTime);
    if (!getScene() || !getScene()->wouldCollide(getBoundsAt(tryY), this)) {
        pos.y = tryY.y;
    } else {
        m_velocity.y = 0.0f;
    }

    // ground
    if (pos.y < -0.5f) { pos.y = -0.5f; m_velocity.y = 0.0f; }

    // rotation
    if (Input::onKeyPress(Key::A)) r += 90.0f * deltaTime;
    if (Input::onKeyPress(Key::D)) r -= 90.0f * deltaTime;
    if (Input::onKeyPress(Key::S)) r = 0.0f;

    if (Input::wasKeyPressed(Key::M)) play("effect");

    if (m_canMove) setPosition(pos);
    setRotation(r);
}

void Player::jump(float y) {
    if (y <= -0.5f) {
        m_velocity.y = JUMP_FORCE;
        if (m_audio) m_audio->playSound("assets/sfx/jump.wav");
    }
}

void Player::flip(glm::vec2& scale, int dir) {
    if ((scale.x < 0 && dir > 0) || (scale.x > 0 && dir < 0))
        scale.x *= -1;
}

