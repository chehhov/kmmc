#pragma once
#include "GameMap.hpp"
#include "GameLevel.hpp"
namespace lae {
    class Engine {
    public:
        Engine();
        ~Engine();
        void run();

    private:
        GameMap map;
        GameLevel level;

        void init_curses();
        void handle_input();
        void flash_screen(bool is_death);
    };
}
