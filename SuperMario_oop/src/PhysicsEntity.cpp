#include "PhysicsEntity.hpp"
namespace lae {
    void PhysicsEntity::init(float x_pos, float y_pos, float w, float h, char type) {
        Entity::init(x_pos, y_pos, w, h, type);
        vertical_speed = 0.0f; 
		horizon_speed = 0.2f; 
		is_fly = false;
    }
    void PhysicsEntity::apply_gravity() {
        is_fly = true;
		vertical_speed += 0.05f;
		y += vertical_speed;
    }
}
