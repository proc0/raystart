#include <entt/entt.hpp>  // https://github.com/skypjack/entt

// Pure data components
struct Position { float x, y; };
struct Velocity { float vx, vy; };
struct PlayerTag {};
struct EnemyTag {};

// Central GameWorld — owns ECS + global state
class GameWorld {
public:
    entt::registry& registry() { return registry_; }  // ECS heart
    float delta_time = 0.f;
    glm::vec2 camera_pos = {0, 0};  // Global state (non-ECS)

    // Helpers: spawn player/enemy
    entt::entity spawn_player(float x, float y) {
        auto e = registry_.create();
        registry_.emplace<PlayerTag>(e);
        registry_.emplace<Position>(e, x, y);
        registry_.emplace<Velocity>(e, 0, 0);
        return e;
    }

    entt::entity spawn_enemy(float x, float y) {
        auto e = registry_.create();
        registry_.emplace<EnemyTag>(e);
        registry_.emplace<Position>(e, x, y);
        registry_.emplace<Velocity>(e, 100, 50);
        return e;
    }

private:
    entt::registry registry_;  // ECS core lives here
};

// Systems — take GameWorld explicitly (clear deps!)
void MovementSystem(GameWorld& world) {
    auto& reg = world.registry();
    auto view = reg.view<Position, Velocity>();
    for (auto e : view) {
        auto& pos = view.get<Position>(e);
        auto& vel = view.get<Velocity>(e);
        pos.x += vel.vx * world.delta_time;
        pos.y += vel.vy * world.delta_time;
    }
}

void EnemyAISystem(GameWorld& world) {
    // Access player via tag query
    auto& reg = world.registry();
    auto player_view = reg.view<PlayerTag, Position>();
    if (player_view.empty()) return;

    auto& player_pos = player_view.get<Position>(player_view.front());
    auto enemy_view = reg.view<EnemyTag, Position, Velocity>();
    for (auto e : enemy_view) {
        auto& pos = enemy_view.get<Position>(e);
        auto& vel = enemy_view.get<Velocity>(e);
        // Simple chase
        vel.vx = (player_pos.x - pos.x) * 0.5f;
        vel.vy = (player_pos.y - pos.y) * 0.5f;
    }
}

// Usage in main loop
int main() {
    GameWorld world;
    world.spawn_player(0, 0);
    world.spawn_enemy(10, 10);

    while (running) {
        world.delta_time = timestep();

        MovementSystem(world);  // Passed explicitly — no globals!
        EnemyAISystem(world);

        // Render: query all Position
        auto view = world.registry().view<Position>();
        for (auto e : view)
            draw(world.registry().get<Position>(e));
    }
}

// Game.h
class Game {
public:
    void update(float dt) {
        input_system.update(*this);
        physics_system.update(*this, dt);
        render_system.update(*this);
    }

    Input& input() { return input_; }
    Player& player() { return player_; }
    entt::registry& registry() { return registry_; }

private:
    Input input_;
    Player player_;
    entt::registry registry_;
};