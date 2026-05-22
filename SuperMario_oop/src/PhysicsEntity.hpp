#pragma once
#include "Entity.hpp"
namespace lae {
    class PhysicsEntity : public Entity {
    public:
        float vertical_speed;
        float horizon_speed;
        bool is_fly;

        void init(float x_pos, float y_pos, float w, float h, char type);
        void apply_gravity();
    };
}
