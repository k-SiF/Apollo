#include "player.h"
#include <apollo/chrono.h>
#include <apollo/input.h>
#include <apollo/key.h>
#include <iostream>

Player::Player(Mesh* mesh, glm::vec2 position) : Entity(mesh, position) {
    setTexture(&tex);
}

void Player::update() {
    handleInput(Time::deltaTime());

}

void Player::handleInput(float deltaTime) {
    float speed = 0.5f;
    glm::vec2 pos = getPosition();
    float r = getRotation();
    if (Input::onKeyPress(Key::Up))    pos.y += 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Left))  pos.x -= 0.5 * deltaTime;    
    if (Input::onKeyPress(Key::Down))  pos.y -= 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Right)) pos.x += 0.5 * deltaTime;
    if (Input::onKeyPress(Key::A))     r += 90.0f * deltaTime;
    if (Input::onKeyPress(Key::D))     r -= 90.0f * deltaTime;
    if (Input::onKeyPress(Key::S))     r = 0.0f;
    if (Input::wasKeyPressed(Key::Space) && pos.y <= -0.5f) {
        m_velocityY = JUMP_FORCE;
    }

    m_velocityY += G * deltaTime;
    pos.y += m_velocityY * deltaTime;

    if (pos.y < -0.5f) {
        pos.y = -0.5f;
        m_velocityY = 0.0f;
    }

    setPosition(pos);
    setRotation(r);
}