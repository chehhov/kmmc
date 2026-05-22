#include "GameMap.hpp"
#include <ncurses.h>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace lae {

GameMap::GameMap(int height, int width)
    : h(height)
    , w(width)
{
    map = new char*[h];

    for (int i = 0; i < h; ++i)
    {
        map[i] = new char[w + 1];
    }
}


GameMap::~GameMap()
{
    for (int i = 0; i < h; ++i)
    {
        delete[] map[i];
    }

    delete[] map;
}


void GameMap::clear()
{
    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            map[j][i] = ' ';
        }

        map[j][w] = '\0';
    }
}


bool GameMap::is_object_on_map(int x, int y) const
{
    return (x >= 0 && x < w && y >= 0 && y < h);
}


void GameMap::put_object_on_map(const Entity& obj)
{
    int ix = static_cast<int>(std::round(obj.x));
    int iy = static_cast<int>(std::round(obj.y));
    int iw = static_cast<int>(std::round(obj.width));
    int ih = static_cast<int>(std::round(obj.height));

    for (int i = ix; i < ix + iw; ++i)
    {
        for (int j = iy; j < iy + ih; ++j)
        {
            if (is_object_on_map(i, j))
            {
                map[j][i] = obj.c_type;
            }
        }
    }
}


void GameMap::put_score_on_map(int score)
{
    char buf[32];
    std::sprintf(buf, "Score: %d", score);
    int len = std::strlen(buf);

    for (int i = 0; i < len && (i + 5) < w; ++i)
    {
        map[0][i + 5] = buf[i];
    }
}


void GameMap::show_map()
{
    for (int j = 0; j < h; ++j)
    {
        mvaddnstr(j, 0, map[j], w);
    }

    refresh();
}

} // namespace lae