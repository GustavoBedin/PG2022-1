#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include <GLFW/glfw3.h>
#include "ball_object.h";
#include "game_object.h";
#include <stdio.h>



SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;

Game::Game(unsigned int width, unsigned int height)
    : Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
}

void Game::Init()
{
    // Carrega shaders.
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    // configura shaders.
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    // Carrega texturas.
    ResourceManager::LoadTexture("textures/background 3.jpg", false, "background");
    ResourceManager::LoadTexture("textures/spoiled_apple2.png", true, "s_apple");
    ResourceManager::LoadTexture("textures/apple.png", true, "apple");
    ResourceManager::LoadTexture("textures/cesta.png", true, "player");

    //Criacao dos objetos.
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));
    glm::vec2 ballPos = glm::vec2(this->Width / 2.0f - 2.0f * BALL_RADIUS / 2.0f, 0 + BALL_RADIUS);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("apple"));

}

void Game::Update(float dt)
{
    //Move a bola
    Ball->Move(dt, this->Width);
    //Checagem de colisoes.
    this->DoCollisions();
    //Se o num de colisoes for multiplo de 5, a maca fica estragada
    if (NUM_COLLISIONS % 5 == 0) {
        Ball->Spoiled = true;
    }
    else {
        Ball->Spoiled = false;
    }
    //Se a maca normal sair da tela, o jogo reinicia.
    if (Ball->Position.y >= this->Height && Ball ->Spoiled == false)
    {
        this->ResetPlayer();
    }
    //Se a maca estragada sair da tela, o jogo continua.
    if (Ball->Position.y >= this->Height && Ball->Spoiled == true) {
        glm::vec2 ballPos = glm::vec2(this->Width / 2.0f - 2.0f * BALL_RADIUS / 2.0f, 0 + BALL_RADIUS);
        NUM_COLLISIONS += 1;
        Ball->Teleport(ballPos);
    }
}

void Game::ProcessInput(float dt)
{
    float velocity = PLAYER_VELOCITY * dt;
        // Movimento do player.
    if (this->Keys[GLFW_KEY_A])
    {
        if (Player->Position.x >= 0.0f)
        {//Movimento para esquerda.
            Player->Position.x -= velocity;
        }
    }
    if (this->Keys[GLFW_KEY_D])
    {
        if (Player->Position.x <= this->Width - Player->Size.x)
        {//Movimento para direita.
            Player->Position.x += velocity;
        }
    }//Barra de espaco comeca o jogo.
    if (this->Keys[GLFW_KEY_SPACE])
        Ball->Stuck = false;
}

void Game::Render()
{
    //Renderizando o background
    Texture2D background;
    background = ResourceManager::GetTexture("background");
    Renderer->DrawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);


    if (Ball->Spoiled == true) {
        //Se maca estiver vencida, mudar a sprite para a da maca podre.
        Ball->Sprite = ResourceManager::GetTexture("s_apple");
    }
    else {
        //Se nao estiver vencida, pegar a sprite da maca normal.
        Ball->Sprite = ResourceManager::GetTexture("apple");
    }
    Ball->Draw(*Renderer);
    Player->Draw(*Renderer);
    //Renderizando player e a maca.
}


void Game::ResetPlayer()
{
    // Reseta o player, bola e numero de colisoes.
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    glm::vec2 ballPos = glm::vec2(this->Width / 2.0f - 2.0f * BALL_RADIUS / 2.0f, 0 + BALL_RADIUS);
    Ball->Reset(ballPos, INITIAL_BALL_VELOCITY);
    NUM_COLLISIONS = 1;
}


bool CheckCollision(GameObject& one, GameObject& two)
{
    // Colisao no x
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // Colisao no y
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;

    return collisionX && collisionY;
}

void Game::DoCollisions()
{
    int randWidth = rand() % this->Width + 1;
    if (CheckCollision(*Ball, *Player) && Ball->Spoiled == false) {
        //Se tem colisao e a maca esta normal, teleporta ela para uma largura aleatoria em cima da janela e aumenta velocidade em 1.
        glm::vec2 ballPos = glm::vec2(randWidth, BALL_RADIUS);
        Ball->IncreaseVelocity(NUM_COLLISIONS);
        NUM_COLLISIONS += 1;
        
        Ball->Teleport(ballPos);
    }
    if (CheckCollision(*Ball, *Player) && Ball->Spoiled == true) {
        //Se tiver colisao e a maca esta vencida, reinicia o jogo.
        ResetPlayer();
    }
}