#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
    auto &projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(id, false);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);
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
