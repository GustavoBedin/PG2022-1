#include "ball_object.h"


BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true), Spoiled(false) { }

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true), Spoiled(false) { }

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // Se o jogo ja comecou
    if (!this->Stuck)
    {
        // A maca se mexe.
        this->Position += this->Velocity * dt;
        this->Rotation += 100 * dt;

    }
    return this->Position;
}

// Reseta a maca.
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Spoiled = false;
    this->Rotation = 0;
}

//Teleporta a maca para uma posicao.
void BallObject::Teleport(glm::vec2 position) {

    this->Position = position;
}

//Aumenta a velocidade da maca.
void BallObject::IncreaseVelocity(int numColl) {
    this->Velocity += glm::vec2(0.0f, numColl);
}