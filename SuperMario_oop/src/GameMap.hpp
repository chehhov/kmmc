#include "Entity.hpp"
namespace lae {
    class GameMap {
    public:
        GameMap(int height, int width);
        ~GameMap();

        void clear();
        bool is_object_on_map(int x, int y) const;
        void put_object_on_map(const Entity& obj);
        void put_score_on_map(int score);
        void show_map();

    private:
        int h, w;
        char** map;
    };
}
