#include "player.h"
#include <apollo/input.h>

Player::Player(Mesh* mesh, glm::vec2 position) : Entity(mesh, position) {

}

void Player::update(float deltaTime) {
    handleInput(deltaTime);
}

void Player::handleInput(float deltaTime) {
    float speed = 0.5f;
    glm::vec2 pos = getPosition();
    if (Input::onKeyPress(Key::Up))    pos.y += 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Left))  pos.x -= 0.5 * deltaTime;    
    if (Input::onKeyPress(Key::Down))  pos.y -= 0.5 * deltaTime;
    if (Input::onKeyPress(Key::Right)) pos.x += 0.5 * deltaTime;
    setPosition(pos);
}