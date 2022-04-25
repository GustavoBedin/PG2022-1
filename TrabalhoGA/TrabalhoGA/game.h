#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


// Tamanho do player.
const glm::vec2 PLAYER_SIZE(100.0f, 40.0f);
// Velocidade do player.
const float PLAYER_VELOCITY(500.0f);

// Velocidade da maca.
const glm::vec2 INITIAL_BALL_VELOCITY(0.0f, 300.0f);
//
// Raio da maca.
const float BALL_RADIUS = 15.5f;

class Game
{
public:
    // game state
    bool         Keys[1024];
    unsigned int Width, Height;
    int NUM_COLLISIONS = 1;
    // Construtor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // Inicializacao (carrega shaders, texturas)
    void Init();
    // Loop de jogo
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    //Checa as colisoes
    void DoCollisions();
    //Reseta o jogo quando a condicao de perda ocorre.
    void ResetPlayer();
};