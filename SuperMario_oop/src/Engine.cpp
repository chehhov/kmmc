#include "Engine.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>


namespace lae {
    Engine::Engine() : map(25, 80), level() {}
    Engine::~Engine() { endwin();}

    void Engine::init_curses() {
        initscr();
		noecho();
		cbreak();
		nodelay(stdscr, TRUE);
		curs_set(0);
        start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		init_pair(2, COLOR_WHITE, COLOR_RED);
        init_pair(3, COLOR_WHITE, COLOR_GREEN);
		bkgd(COLOR_PAIR(1));
		clear();
		refresh();
    }


    void Engine::handle_input() {
        int c = getch();
		
		if (c == ERR) {
			return;
		}
		
        if (c == 27) {
			exit(0);
		}
		
        if (c == ' ') {
			level.mario.vertical_speed = -1.0f;
		}
		
        if (c == 'a' || c == 'A') {
			level.horizon_move_map(2.0f);
		}
		
        if (c == 'd' || c == 'D') {
			level.horizon_move_map(-2.0f);
		}
    }


    void Engine::flash_screen(bool is_death) {
        short color = is_death ? 2 : 3;
        bkgd(COLOR_PAIR(color));
		clear();
		refresh();
		napms(500);
        bkgd(COLOR_PAIR(1));
		clear();
		refresh();
    }


    void Engine::run() {
        init_curses(); 
		level.create_level(1);
        while (true) {
            map.clear();
			napms(16);
			handle_input();
            if (level.is_player_out_of_the_map()) {
				flash_screen(true);
				level.create_level(level.level);
			}
            level.vertical_move(&level.mario);
			level.check_mario_collisions();
            for (int i = 0; i < level.movingLength; ++i) {
                level.vertical_move(&level.moving[i]);
				level.horizon_move_object(&level.moving[i]);
				
                if (level.moving[i].y > 25) {
					level.remove_moving(i);
					--i;
					continue; 
				}
            }
			
            level.render(map);
			map.show_map();
            if (getch() == 27) {
				break;
			}
        }
    }
}
