#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"



class BallObject : public GameObject
{
public:
    float   Radius;
    bool    Stuck;
    bool    Spoiled;
    // Construtores.
    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    // Movimenta a maca.
    glm::vec2 Move(float dt, unsigned int window_width);
    // Reseta a bola com a velocidade e posicao original.
    void      Reset(glm::vec2 position, glm::vec2 velocity);

    void      Teleport(glm::vec2 position);

    void      IncreaseVelocity(int numColl);
};

#endif