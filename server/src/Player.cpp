#include "Player.h"

#include "mathutils.hpp"

Player::Player() : position(0.0f), color(math::randf(), math::randf(), math::randf()) {

}

void Player::move(const glm::vec3& translation) {
    position += translation;
}

void* Player::serialize() const {
    float* data = (float*) malloc(PLAYER_SERIALIZATION_SIZE);
    data[0] = position.x;
    data[1] = position.y;
    data[2] = position.z;
    data[3] = color.x;
    data[4] = color.y;
    data[5] = color.z;
    return data;
}