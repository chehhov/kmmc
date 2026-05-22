#pragma once
#include "PhysicsEntity.hpp"
namespace lae {
    class GameMap;
    class GameLevel {
    public:
        GameLevel();
        ~GameLevel();

        void create_level(int lvl);
        void remove_moving(int i);
        void vertical_move(PhysicsEntity* obj);
        void horizon_move_map(float dx);
        void horizon_move_object(PhysicsEntity* obj);
        void check_mario_collisions();
        bool is_player_out_of_the_map() const;
        void render(GameMap& map) const;

        PhysicsEntity mario;
        int score;
        int level;
        int maxLvl;
        
        Entity* bricks;
        int brickLength;
        
        PhysicsEntity* moving;
        int movingLength;
    };
}   
