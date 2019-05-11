#include "AssetManager.h"
#include "ECS/Components.h"
#include <memory>

static int srcSize = 100;
static float scale = 0.24;

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Entity &entity, Game::Roles type, char option)
{
    auto &projectile(manager->addEntity());
    
    switch (type) {
        case Game::playerRole:
            AssetManager::CreateProjectile(projectile, entity, 500, 5, 50, "fireball", option);
            projectile.getComponent<SpriteComponent>().addAnimation(0, 2, 600, "LightUp");
            projectile.addGroup(Game::groupPlayerProjectiles);
            break;
        case Game::enemyRole:
            AssetManager::CreateProjectile(projectile, entity, 500, pSpeed, 50, "projectile", option);
            projectile.addGroup(Game::groupProjectiles);
            break;
        default:
            break;
    }
}

void AssetManager::CreateProjectile(Entity &projectile, Entity &player, int range, int speed, int dmg, std::string id, char option)
{
    auto &vel = player.getComponent<TransformComponent>().orientation;
    std::unique_ptr<Vector2D> v = std::make_unique<Vector2D>(vel.x, vel.y);
    auto &pos = player.getComponent<TransformComponent>().position;
    std::unique_ptr<Vector2D> dim(player.getComponent<SpriteComponent>().getSize());
    int x = (vel.x == 0.0f) ? dim->x / 2 : dim->x;
    int y = (vel.y == 0.0f) ? dim->y / 2 : dim->y;
    int flagX = static_cast<int>(vel.x != -1.0f);
    int flagY = static_cast<int>(vel.y != -1.0f);
    projectile.addComponent<TransformComponent>(pos.x + flagX * x, pos.y + flagY * y, srcSize, srcSize, scale);
    projectile.addComponent<SpriteComponent>(id, true);
    switch (option) {
        case 'D':
            v->x = (v->x == 0) ? -1 : v->x;
            v->y = (v->y == 0) ? -1 : v->y;
            break;
        case 'U':
            v->x = (v->x == 0) ? 1 : v->x;
            v->y = (v->y == 0) ? 1 : v->y;
            break;
        case 'N':
            break;
        default:
            break;
            
    }
    projectile.addComponent<ProjectileComponent>(range, speed, *v, dmg);
    projectile.addComponent<ColliderComponent>(id);
}

void AssetManager::AddTexture(std::string ID, const char *path)
{
    textures.emplace(ID, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string ID)
{
    return textures[ID];
}

void AssetManager::AddFont(std::string ID, std::string path, int fontSize)
{
    fonts.emplace(ID, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *AssetManager::GetFont(std::string ID)
{
    return fonts[ID];
}
