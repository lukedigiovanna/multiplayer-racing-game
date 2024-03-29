#pragma once

#include <glm/glm.hpp>

#define PLAYER_SERIALIZATION_SIZE 6 * sizeof(float)

class Player {
private:
    glm::vec3 position;
    glm::vec3 color;
public:
    Player();

    void move(const glm::vec3& translation);

    void* serialize() const;
};