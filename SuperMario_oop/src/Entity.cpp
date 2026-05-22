#include "Entity.hpp"


namespace lae {
    void Entity::init(float x_pos, float y_pos, float w, float h, char type) {
        x = x_pos; 
		y = y_pos;
		width = w;
		height = h;
		c_type = type;
    }
	
	
    void Entity::set_position(float x_pos, float y_pos) {
		x = x_pos;
		y = y_pos; 
	}
	
	
    bool Entity::is_collision(const Entity& other) const {
        return (x + width > other.x) && (x < other.x + other.width) &&
               (y + height > other.y) && (y < other.y + other.height);
    }
}
