#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>


Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager *Game::assets = new AssetManager(&manager);

//std::vector<ColliderComponent *> Game::colliders;

bool Game::isRunning = false;

Entity &player(manager.addEntity());
Entity &label(manager.addEntity());

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
auto &colliders(manager.getGroup(Game::groupColliders));
auto &projectiles(manager.getGroup(Game::groupProjectiles));
auto &playerProj(manager.getGroup(Game::groupPlayerProjectiles));

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialized..." << std::endl;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        
        if (window)
        {
            std::cout << "window created!" << std::endl;
        }
        
        renderer = SDL_CreateRenderer(window, -1, flags);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }
        isRunning = true;
    }
    
    if (TTF_Init() == -1)
    {
        std::cout << "TTF_Init failed" << std::endl;
        return;
    }
    
    assets->AddTexture("terrain", "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/terrain_ss.png");
    assets->AddTexture("player", "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/dragon2.png");
    assets->AddTexture("projectile", "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/proj.png");
    assets->AddTexture("fireball", "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/fireball.png");
    
    assets->AddFont("arial", "/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/arial.ttf", 16);
    
    map = new Map("terrain", 2, 32);
    
    //ecs system
    map->LoadMap("/Users/yuqiliu/Documents/Dev/2D_Game_Engine/2D_Game_Engine/assets/map.map", 25, 20);

    
    player.addComponent<TransformComponent>(191, 161, 0.5);
    player.addComponent<SpriteComponent>("player", true);
    
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    
    SDL_Color white = { 255, 255, 255, 255 };
    label.addComponent<UILabel>(10, 10, "Test String", "arial", white);
}

void Game::handleEvents()
{
    
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update()
{
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    
    std::stringstream ss;
    
    ss << "Player position: " << playerPos;
    
    label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

    manager.refresh();
    manager.update();
    
    for (auto &c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol))
        {
            player.getComponent<TransformComponent>().position = playerPos;
            if (c->getComponent<ColliderComponent>().tag == "terrain")
            {
                player.getComponent<TransformComponent>().position -= player.getComponent<TransformComponent>().velocity;

            }
        }
        
    }
    
    for (auto &p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player!" << std::endl;
            p->destroy();
        }
    }
    
    for (auto &p : playerProj)
    {
    }
    
    camera.x = player.getComponent<TransformComponent>().position.x - 300;
    camera.y = player.getComponent<TransformComponent>().position.y - 220;
    
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > map->mapW - camera.w) camera.x = camera.w;
    if (camera.y > map->mapH - camera.h) camera.y = camera.h;
    
}


void Game::render()
{
    SDL_RenderClear(renderer);
    int xpos, ypos;
    bool inXRange, inYRange;
    
    for (auto &t : tiles)
    {
        xpos = t->getComponent<TileComponent>().position.x;
        ypos = t->getComponent<TileComponent>().position.y;
        
        inXRange = (camera.x - map->scaledSize <= xpos)
        && (xpos <= camera.x + map->scaledSize + camera.w);
        inYRange = (camera.y - map->scaledSize <= ypos)
        && (ypos <= camera.y + map->scaledSize + camera.h);
        
        if (inXRange && inYRange) t->draw();
    }
    
    for (auto &p : players) p->draw();
    
    for (auto &p : projectiles) p->draw();
    
    for (auto &p : playerProj) p->draw();
    
    label.draw();
    
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
