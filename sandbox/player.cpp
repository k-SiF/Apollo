#include "player.h"
#include <apollo/chrono.h>
#include <apollo/scene.h>
#include <apollo/input.h>
#include <apollo/key.h>
#include <iostream>

Player::Player(Mesh* mesh, glm::vec2 position) : Entity(mesh, position) {
    setTextureSize(&tex, 0.7f);
    setTexture(&tex);
}

void Player::update(float deltaTime) {
    if (Input::wasKeyPressed(Key::Space))  m_jump = true;
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
    glm::vec2 scale = getScale();
    setColScale(glm::vec2(0.1f, 0.45f));
    float r = getRotation();

    // --- horizontal input ---
    float moveX = 0.0f;
    if (Input::onKeyPress(Key::Left))  { moveX = -SPEED; flip(scale, -1); }
    if (Input::onKeyPress(Key::Right)) { moveX =  SPEED; flip(scale,  1); }

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

    // ground floor (optional, if you still want a hard floor)
    if (pos.y < -0.5f) { pos.y = -0.5f; m_velocity.y = 0.0f; }

    // rotation
    if (Input::onKeyPress(Key::A)) r += 90.0f * deltaTime;
    if (Input::onKeyPress(Key::D)) r -= 90.0f * deltaTime;
    if (Input::onKeyPress(Key::S)) r = 0.0f;

    if (m_canMove) setPosition(pos);
    setScale(scale);
    setRotation(r);
}

void Player::jump(float y) {
    if (y <= -0.5f) {
        m_velocity.y = JUMP_FORCE;
    }
}

void Player::flip(glm::vec2& scale, int dir) {
    if ((scale.x < 0 && dir > 0) || (scale.x > 0 && dir < 0))
        scale.x *= -1;
}

