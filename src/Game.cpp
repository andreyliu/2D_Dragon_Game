#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "Enemies.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map *map;
Manager manager;
Enemies *enemiesLoader;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager *Game::assets = new AssetManager(&manager);

int health = 1000;
static int damage = 10;
int score = 0;

//std::vector<ColliderComponent *> Game::colliders;

bool Game::isRunning = false;

Entity &player(manager.addEntity());
Entity &label(manager.addEntity());

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
std::vector<Entity *> &enemies(manager.getGroup(Game::groupEnemies));

auto &colliders(manager.getGroup(Game::groupColliders));
auto &projectiles(manager.getGroup(Game::groupProjectiles));
auto &playerProj(manager.getGroup(Game::groupPlayerProjectiles));

std::string Game::path = "";

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
    
    assets->AddTexture("terrain", (Game::path + "/src/assets/terrain_ss.png").c_str());
    assets->AddTexture("player", (Game::path + "/src/assets/dragon2.1.png").c_str());
    assets->AddTexture("projectile", (Game::path + "/src/assets/proj.png").c_str());
    assets->AddTexture("fireball", (Game::path + "/src/assets/fireball.png").c_str());
    assets->AddTexture("enemy", (Game::path + "/src/assets/enemy.png").c_str());
    assets->AddTexture("flame", (Game::path + "/src/assets/flame.png").c_str());
    assets->AddFont("SLB", Game::path + "/src/assets/SuperLegendBoy.ttf", 16);
    map = new Map("terrain", 2, 32);
    
    //ecs system
    map->LoadMap(Game::path + "/src/assets/map.map", 25, 20);
    std::cout << "map created" << std::endl;

    enemiesLoader = new Enemies();
    enemiesLoader->LoadEnemies(Game::path + "/src/assets/enemies.map", 25, 20);
    std::cout << "enemies created" << std::endl;
    
    player.addComponent<TransformComponent>(191, 161, 0.5);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    std::cout << "Initilized player" << std::endl;
    
    SDL_Color white = { 255, 255, 255, 255 };
    label.addComponent<UILabel>(10, 10, "Test String", "SLB", white);
    std::cout << "game labels created" << std::endl;
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
    
    ss << "HEALTH: " << health;
    
    label.getComponent<UILabel>().SetLabelText(ss.str(), "SLB");

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

    for (auto &c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (c->getComponent<ColliderComponent>().tag != "enemy")
        {
            continue;
        }
        for (auto &c2 : colliders)
        {
            if (c2->getComponent<ColliderComponent>().tag != "enemy")
            {
                continue;
            }
            SDL_Rect c2Col = c2->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(cCol, c2Col))
            {
                c->getComponent<TransformComponent>().position -= c->getComponent<TransformComponent>().velocity;
                c2->getComponent<TransformComponent>().position -= c2->getComponent<TransformComponent>().velocity;
                
            }

        }
        
    }

    for (auto &p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
        {
            p->destroy();
            health -= damage;
            score--;
        }
    }

    for (auto &e : enemies)
    {
        int xpos = static_cast<int>(e->getComponent<TransformComponent>().position.x);
        int ypos = static_cast<int>(e->getComponent<TransformComponent>().position.y);
        bool onMap = xpos > 0 && ypos > 0 && map->mapW > xpos && map->mapH > ypos;
        if (!onMap)
        {
            e->destroy();
        }
    }

    for (auto &p : playerProj)
    {
        for (auto &e: enemies)
        {
            if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
            {
                p->destroy();
                e->destroy();
                score++;
            }
            
        }
        
        for (auto &ep : projectiles)
        {
            if (Collision::AABB(ep->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
            {
                ep->destroy();
            }
        }
    }

    camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 300);
    camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 220);
    
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > map->mapW - camera.w) camera.x = camera.w;
    if (camera.y > map->mapH - camera.h) camera.y = camera.h;

}

template<typename T> bool Game::inView(Entity *t)
{
    int xpos, ypos;
    bool inXRange, inYRange;
    xpos = t->getComponent<T>().position.x;
    ypos = t->getComponent<T>().position.y;
    
    inXRange = (camera.x - map->scaledSize <= xpos)
    && (xpos <= camera.x + map->scaledSize + camera.w);
    inYRange = (camera.y - map->scaledSize <= ypos)
    && (ypos <= camera.y + map->scaledSize + camera.h);
    
    return inXRange && inYRange;
    
}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    for (auto &t : tiles)
    {
        if (Game::inView<TileComponent>(t)) t->draw();
    }
    
    for (auto &e : enemies)
    {
        if (Game::inView<TransformComponent>(e)) e ->draw();
    }
    
    for (auto &p : projectiles) p->draw();
    
    for (auto &p : playerProj) p->draw();
    
    for (auto &p : players) p->draw();
    
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

void Game::displayScore()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    if (SDL_RenderClear(renderer) != 0)
    {
        std::cout << "Renderer failed to clear!" << std::endl;
    }
    SDL_Color white = { 255, 255, 255, 255 };
    std::string EGText = std::string("GAME OVER");
    std::string Score = std::string("Score: ") + std::to_string(score);
    std::unique_ptr<UILabel> EGLabel = std::make_unique<UILabel>(320, 200, EGText, "SLB", white);
    std::unique_ptr<UILabel> ScoreLabel = std::make_unique<UILabel>(340, 350, Score, "SLB", white);
    std::unique_ptr<UILabel> MsgLabel = std::make_unique<UILabel>(580, 600, "press ESC to exit", "SLB", white);
    EGLabel->draw();
    ScoreLabel->draw();
    MsgLabel->draw();
    SDL_RenderPresent(renderer);
}
