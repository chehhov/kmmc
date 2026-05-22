#pragma once
namespace lae {
    class Entity {
    public:
        float x, y;
        float width, height;
        char c_type;

        void init(float x_pos, float y_pos, float w, float h, char type);
        void set_position(float x_pos, float y_pos);
        bool is_collision(const Entity& other) const;
    };
}
