#include "AssetManager.h"
#include "ECS/Components.h"
#include <memory>
#include "SDL2/SDL.h"

static int fbSrcSize = 100;
static float fbScale = 0.24;

static int eSrcSize = 32;
static float eScale = 0.5;

static int flameSrcSize = 45;
static float flameScale = 1;

static int side_fireball_offset = 30;

static int Player_PRange = 300;
static float Player_PSpeed = 4;
static int Player_PDamage = 50;

static int Enemy_PRange = 200;
static float Enemy_PSpeed = 4;
static int Enemy_PDamage = 10;
static int Enemy_SensingDist = 400;

AssetManager::AssetManager(Manager *man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Entity &entity, Game::PType type, char option, bool sideOffset)
{
    auto &projectile(manager->addEntity());
    switch (type) {
        case Game::playerT:
            AssetManager::P_FireballTransform(projectile, entity, fbSrcSize, fbScale,
                                              entity.getComponent<TransformComponent>().orientation, option, sideOffset);
            projectile.addComponent<ColliderComponent>("fireball");
            projectile.addComponent<SpriteComponent>("fireball", true);
            projectile.getComponent<SpriteComponent>().addAnimation(0, 2, 600, "LightUp");
            projectile.addComponent<ProjectileComponent>(Player_PRange, Player_PSpeed, Player_PDamage);
            projectile.addGroup(Game::groupPlayerProjectiles);
            break;
            
        case Game::playerFlameT:

            AssetManager::ProjTransform(projectile, entity, flameSrcSize, flameScale,
                                        *Vector2D::ZeroVector);
            projectile.addComponent<SpriteComponent>("flame", true);
            projectile.getComponent<SpriteComponent>().addAnimation(0, 2, 600, "Burn");
            projectile.addComponent<ProjectileComponent>(600, 1, 100);
            projectile.addComponent<ColliderComponent>("flame");
            projectile.addGroup(Game::groupPlayerProjectiles);
            projectile.getComponent<TransformComponent>().position.x += entity.getComponent<TransformComponent>().orientation.x * 50;
            projectile.getComponent<TransformComponent>().position.y += entity.getComponent<TransformComponent>().orientation.y * 50;
            break;

        case Game::enemyBasicT:

            AssetManager::ProjTransform(projectile, entity, eSrcSize, eScale,
                                        entity.getComponent<TransformComponent>().velocity);
            projectile.addComponent<SpriteComponent>("projectile", false);
            projectile.addComponent<ProjectileComponent>(Enemy_PRange, Enemy_PSpeed, Enemy_PDamage);
            projectile.addComponent<ColliderComponent>("projectile");
            projectile.addGroup(Game::groupProjectiles);
            break;

        default:
            break;
    }
}

void AssetManager::CreateEnemy(float walkR, int xpos, int ypos, int xvel, int yvel, Game::PType type, std::string id, int seed)
{
    auto &enemy(manager->addEntity());

    switch (type) {

        case Game::enemyBasicT:

            enemy.addComponent<TransformComponent>(xpos, ypos, 32, 32, 2);
            enemy.getComponent<TransformComponent>().velocity.x = xvel;
            enemy.getComponent<TransformComponent>().velocity.y = yvel;
            enemy.getComponent<TransformComponent>().velocity.Normalize();
            AssetManager::CreateEnemy(enemy, walkR, 0.2, 0.2, 0.5, id);
            enemy.getComponent<EnemyComponent>().enemyID = seed;
            break;

        case Game::enemyGeneralT:

            break;

        default:
            break;
    }
}

void AssetManager::CreateEnemy(Entity &enemy, float walkR, float walkS, float projS, float followS, std::string id)
{
    enemy.addComponent<SpriteComponent>(id, true);
    enemy.getComponent<SpriteComponent>().addAnimation(0, 3, 100, "Idle");
    enemy.getComponent<SpriteComponent>().addAnimation(1, 6, 100, "Walk");
    
    enemy.addComponent<ColliderComponent>(id);
    enemy.addComponent<EnemyComponent>(walkR, walkS, projS, followS, Enemy_SensingDist, Enemy_PRange);
    enemy.addGroup(Game::groupEnemies);
}

void AssetManager::ProjTransform(Entity &projectile, Entity &player, int srcSize, float scale, Vector2D &vel)
{
    auto &pos = player.getComponent<TransformComponent>().position;
    std::unique_ptr<Vector2D> dim(player.getComponent<SpriteComponent>().getSize());
    int x = static_cast<int>((vel.x == 0.0f) ? dim->x / 2 : dim->x);
    int y = static_cast<int>((vel.y == 0.0f) ? dim->y / 2 : dim->y);
    int flagX = static_cast<int>(vel.x != -1.0f);
    int flagY = static_cast<int>(vel.y != -1.0f);
    projectile.addComponent<TransformComponent>(pos.x + flagX * x, pos.y + flagY * y, srcSize, srcSize, scale);

    projectile.getComponent<TransformComponent>().velocity = vel;
}

void AssetManager::P_FireballTransform(Entity &projectile, Entity &player, int srcSize, float scale, Vector2D &vel,
                                       char option, bool sideOffset)
{
    auto &pos = player.getComponent<TransformComponent>().position;
    std::unique_ptr<Vector2D> dim(player.getComponent<SpriteComponent>().getSize());
    int x = static_cast<int>((vel.x == 0.0f) ? dim->x / 2 : dim->x);
    int y = static_cast<int>((vel.y == 0.0f) ? dim->y / 2 : dim->y);
    int flagX = static_cast<int>(vel.x != -1.0f);
    int flagY = static_cast<int>(vel.y != -1.0f);
    projectile.addComponent<TransformComponent>(pos.x + flagX * x, pos.y + flagY * y, srcSize, srcSize, scale);
    std::cout << "************************************fireballaddTransform" << std::endl;
    std::unique_ptr<Vector2D> v = std::make_unique<Vector2D>(vel.x, vel.y);

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
    projectile.getComponent<TransformComponent>().velocity = *v;
    if (sideOffset)
    {
        projectile.getComponent<TransformComponent>().position.y += side_fireball_offset;
    }

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
