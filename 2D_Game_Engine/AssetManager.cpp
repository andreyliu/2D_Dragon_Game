#include "AssetManager.h"
#include "ECS/Components.h"
#include <memory>

static int srcSize = 100;
static float scale = 0.24;

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Entity &entity, Game::Roles type)
{
    auto &projectile(manager->addEntity());
    
    switch (type) {
        case Game::playerRole:
            AssetManager::CreateProjectile(projectile, entity, 500, 5, 50, "fireball");
            projectile.getComponent<SpriteComponent>().addAnimation(0, 2, 600, "LightUp");
            projectile.addGroup(Game::groupPlayerProjectiles);
            break;
        case Game::enemyRole:
            AssetManager::CreateProjectile(projectile, entity, 500, pSpeed, 50, "projectile");
            projectile.addGroup(Game::groupProjectiles);
            break;
        default:
            break;
    }
}

void AssetManager::CreateProjectile(Entity &projectile, Entity &player, int range, int speed, int dmg, std::string id)
{
//    int radius = static_cast<int>(srcSize * scale) / 2;
//    std::unique_ptr<DisplaySize> playerSize(player.getComponent<SpriteComponent>().getSize());
//    projectile.addComponent<TransformComponent>(pos.x + playerSize->width / 2 - radius, pos.y + playerSize->height / 2 - radius, srcSize, srcSize, scale);
    auto &vel = player.getComponent<TransformComponent>().orientation;
    projectile.addComponent<TransformComponent>(srcSize, srcSize, scale);
    projectile.addComponent<SpriteComponent>(id, true);
    projectile.addComponent<ProjectileComponent>(player, range, speed, vel, dmg);
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
