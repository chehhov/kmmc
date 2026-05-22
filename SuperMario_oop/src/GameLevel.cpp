#include "GameLevel.hpp"
#include "GameMap.hpp"
#include "Symbols.hpp"
#include <cstdlib>
#include <cmath>

namespace lae {

GameLevel::GameLevel()
    : score(0)
    , level(1)
    , maxLvl(3)
    , bricks(nullptr)
    , brickLength(0)
    , moving(nullptr)
    , movingLength(0)
{
}


GameLevel::~GameLevel()
{
    std::free(bricks);
    std::free(moving);
}


void GameLevel::create_level(int lvl)
{
    level = lvl;
    brickLength = 0;
    std::free(bricks);
    bricks = nullptr;
    movingLength = 0;
    std::free(moving);
    moving = nullptr;

    mario.init(39, 10, 3, 3, Symbols::MARIO);
    score = 0;

    auto add_brick = [&](float x, float y, float w, float h, char t)
    {
        brickLength++;
        bricks = static_cast<Entity*>(std::realloc(bricks, sizeof(Entity) * brickLength));
        bricks[brickLength - 1].init(x, y, w, h, t);
    };

    auto add_moving = [&](float x, float y, float w, float h, char t)
    {
        movingLength++;
        moving = static_cast<PhysicsEntity*>(std::realloc(moving, sizeof(PhysicsEntity) * movingLength));
        moving[movingLength - 1].init(x, y, w, h, t);
    };

    switch (lvl)
    {
        case 1:
            add_brick(20, 20, 40, 5, Symbols::BRICK);
            add_brick(60, 15, 40, 10, Symbols::BRICK);
            add_brick(100, 20, 20, 5, Symbols::BRICK);
            add_brick(120, 15, 10, 10, Symbols::BRICK);
            add_brick(150, 20, 40, 5, Symbols::BRICK);
            add_brick(30, 10, 5, 3, Symbols::QUESTION);
            add_brick(50, 10, 5, 3, Symbols::QUESTION);
            add_brick(70, 5, 5, 3, Symbols::QUESTION);
            add_brick(80, 5, 5, 3, Symbols::QUESTION);
            add_brick(60, 5, 10, 3, Symbols::SOLID);
            add_brick(75, 5, 5, 3, Symbols::SOLID);
            add_brick(85, 5, 10, 3, Symbols::SOLID);
            add_brick(210, 15, 10, 10, Symbols::FINISH);
            add_moving(25, 10, 3, 2, Symbols::ENEMY);
            add_moving(80, 10, 3, 2, Symbols::ENEMY);
            break;

        case 2:
            add_brick(20, 20, 40, 5, Symbols::BRICK);
            add_brick(60, 15, 10, 10, Symbols::BRICK);
            add_brick(80, 20, 20, 5, Symbols::BRICK);
            add_brick(120, 15, 10, 10, Symbols::BRICK);
            add_brick(150, 20, 40, 5, Symbols::BRICK);
            add_brick(210, 15, 10, 10, Symbols::FINISH);
            add_moving(25, 10, 3, 2, Symbols::ENEMY);
            add_moving(80, 10, 3, 2, Symbols::ENEMY);
            add_moving(65, 10, 3, 2, Symbols::ENEMY);
            add_moving(120, 10, 3, 2, Symbols::ENEMY);
            add_moving(160, 10, 3, 2, Symbols::ENEMY);
            add_moving(175, 10, 3, 2, Symbols::ENEMY);
            break;

        case 3:
            add_brick(20, 20, 40, 5, Symbols::BRICK);
            add_brick(80, 20, 15, 5, Symbols::BRICK);
            add_brick(120, 15, 15, 10, Symbols::BRICK);
            add_brick(160, 10, 15, 15, Symbols::FINISH);
            add_moving(25, 10, 3, 2, Symbols::ENEMY);
            add_moving(50, 10, 3, 2, Symbols::ENEMY);
            add_moving(80, 10, 3, 2, Symbols::ENEMY);
            add_moving(90, 10, 3, 2, Symbols::ENEMY);
            add_moving(120, 10, 3, 2, Symbols::ENEMY);
            add_moving(130, 10, 3, 2, Symbols::ENEMY);
            break;

        default:
            break;
    }
}


void GameLevel::remove_moving(int i)
{
    movingLength--;
    moving[i] = moving[movingLength];
    moving = static_cast<PhysicsEntity*>(std::realloc(moving, sizeof(PhysicsEntity) * movingLength));
}


void GameLevel::vertical_move(PhysicsEntity* obj)
{
    obj->is_fly = true;
    obj->vertical_speed += 0.05f;
    obj->y += obj->vertical_speed;

    for (int i = 0; i < brickLength; ++i)
    {
        if (obj->is_collision(bricks[i]))
        {
            if (obj->vertical_speed > 0)
            {
                obj->is_fly = false;
            }

            if (bricks[i].c_type == Symbols::QUESTION && obj->vertical_speed < 0 && obj == &mario)
            {
                bricks[i].c_type = Symbols::SOLID;
                movingLength++;
                moving = static_cast<PhysicsEntity*>(std::realloc(moving, sizeof(PhysicsEntity) * movingLength));
                moving[movingLength - 1].init(bricks[i].x, bricks[i].y - 3, 3, 2, Symbols::COIN);
                moving[movingLength - 1].vertical_speed = -0.7f;
            }

            obj->y -= obj->vertical_speed;
            obj->vertical_speed = 0;
            obj->is_fly = false;

            if (bricks[i].c_type == Symbols::FINISH && obj == &mario)
            {
                level++;

                if (level > maxLvl)
                {
                    level = 1;
                }

                create_level(level);
            }

            break;
        }
    }
}


void GameLevel::horizon_move_map(float dx)
{
    mario.x -= dx;

    for (int i = 0; i < brickLength; ++i)
    {
        if (mario.is_collision(bricks[i]))
        {
            mario.x += dx;
            return;
        }
    }

    mario.x += dx;

    for (int i = 0; i < brickLength; ++i)
    {
        bricks[i].x += dx;
    }

    for (int i = 0; i < movingLength; ++i)
    {
        moving[i].x += dx;
    }
}


void GameLevel::horizon_move_object(PhysicsEntity* obj)
{
    obj->x += obj->horizon_speed;

    for (int i = 0; i < brickLength; ++i)
    {
        if (obj->is_collision(bricks[i]))
        {
            obj->x -= obj->horizon_speed;
            obj->horizon_speed = -obj->horizon_speed;
            return;
        }
    }

    if (obj->c_type == 'o' || obj->c_type == '$')
    {
        PhysicsEntity tmp = *obj;
        tmp.is_fly = true;
        tmp.vertical_speed += 0.05f;
        tmp.y += tmp.vertical_speed;

        bool falling = true;

        for (int i = 0; i < brickLength; ++i)
        {
            if (tmp.is_collision(bricks[i]))
            {
                falling = false;
                break;
            }
        }

        if (falling)
        {
            obj->x -= obj->horizon_speed;
            obj->horizon_speed = -obj->horizon_speed;
        }
    }
}


void GameLevel::check_mario_collisions()
{
    for (int i = 0; i < movingLength; ++i)
    {
        if (mario.is_collision(moving[i]))
        {
            if (moving[i].c_type == 'o')
            {
                if (mario.is_fly && mario.vertical_speed > 0 && (mario.y + mario.height < moving[i].y + moving[i].height * 0.5f))
                {
                    score += 50;
                    remove_moving(i);
                    --i;
                    continue;
                }
                else
                {
                    create_level(level);
                }
            }
            else if (moving[i].c_type == '$')
            {
                score += 100;
                remove_moving(i);
                --i;
                continue;
            }
        }
    }
}


bool GameLevel::is_player_out_of_the_map() const
{
    return mario.y > 25;
}


void GameLevel::render(GameMap& map) const
{
    for (int i = 0; i < brickLength; ++i)
    {
        map.put_object_on_map(bricks[i]);
    }

    for (int i = 0; i < movingLength; ++i)
    {
        map.put_object_on_map(moving[i]);
    }

    map.put_object_on_map(mario);
    map.put_score_on_map(score);
}

} 