#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>


struct GameObject {
    float x, y;
    float width, height;
    float vertSpeed;
    bool IsFly;
    char cType;
    float horizSpeed;
};


GameObject* GetNewBrick(int &brickLength, GameObject* &brick);
GameObject* GetNewMoving(int &movingLength, GameObject* &moving);
void clear_map(char** map, int MAP_HEIGHT, int MAP_WIDTH);
void delete_moving(int &movingLength, GameObject* &moving, int i);
void horizon_move_map(GameObject &mario, int brickLength, GameObject brick[], int movingLength, GameObject moving[], float dx);
void horizon_move_object(GameObject *obj, int &brickLength, GameObject* &brick, int &level, int &maxLvl, GameObject &mario, int &movingLength, GameObject* &moving, int &score);
void init_object(GameObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
bool is_collision(GameObject o1, GameObject o2);
bool is_pos_in_map(int x, int y, int MAP_WIDTH, int MAP_HEIGHT);
void mario_collision(GameObject &mario, int &movingLength, GameObject* &moving, int &score, int &level, int &maxLvl, int &brickLength, GameObject* &brick);
void player_dead(int &level, int &maxLvl, int &brickLength, int &movingLength, GameObject* &brick, GameObject* &moving, GameObject &mario, int &score);
void put_object_on_map(char** map, int MAP_HEIGHT, int MAP_WIDTH, GameObject obj);
void put_score_on_map(char** map, int MAP_WIDTH, int score);
void set_object_pos(GameObject *obj, float xPos, float yPos);
void show_map(char** map, int MAP_HEIGHT, int MAP_WIDTH);
void set_cur(int x, int y);
void vert_move_object(GameObject *obj, int &brickLength, GameObject* &brick, int &level, int &maxLvl, GameObject &mario, int &movingLength, GameObject* &moving, int &score);
void create_level(int &maxLvl, int &brickLength, int &movingLength, GameObject* &brick, GameObject* &moving, GameObject &mario, int &score, int lvl);


int main() {
    const int MAP_WIDTH = 80;
    const int MAP_HEIGHT = 25;
    
    char** map = new char*[MAP_HEIGHT];
    for (int i = 0; i < MAP_HEIGHT; i++) {
        map[i] = new char[MAP_WIDTH + 1];
    }
    
    int brickLength = 0;
    int movingLength = 0;
    int level = 1;
    int score = 0;
    int maxLvl = 3;
    GameObject mario;
    GameObject *brick = nullptr;
    GameObject *moving = nullptr;

    create_level(maxLvl, brickLength, movingLength, brick, moving, mario, score, level);
    
    struct termios oldt, newt; 
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt; 
    newt.c_lflag &= ~(ICANON | ECHO); 
    newt.c_cc[VMIN] = 0; 
    newt.c_cc[VTIME] = 0; 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    printf("\033[44m\033[2J");
    printf("\033[?25l"); 
    fflush(stdout);
    
    do {
        clear_map(map, MAP_HEIGHT, MAP_WIDTH);
        
        {
            fd_set f; 
            struct timeval t = {0, 16000}; 
            FD_ZERO(&f); 
            FD_SET(0, &f); 
            select(1, &f, 0, 0, &t); 
            char c; 
            if (read(0, &c, 1) == 1) {
                if (c == 27) {
                    break;
                }
                if (c == ' ') {
                    mario.vertSpeed = -1;
                }
                if (c == 'a' || c == 'A') {
                    horizon_move_map(mario, brickLength, brick, movingLength, moving, 2);
                }
                if (c == 'd' || c == 'D') {
                    horizon_move_map(mario, brickLength, brick, movingLength, moving, -2);
                }
            }
        }
        
        if (mario.y > MAP_HEIGHT) {
            player_dead(level, maxLvl, brickLength, movingLength, brick, moving, mario, score);
        }
        
        vert_move_object(&mario, brickLength, brick, level, maxLvl, mario, movingLength, moving, score);
        mario_collision(mario, movingLength, moving, score, level, maxLvl, brickLength, brick);
        
        for (int i = 0; i < brickLength; i++) {
            put_object_on_map(map, MAP_HEIGHT, MAP_WIDTH, brick[i]);
        }
        
        for (int i = 0; i < movingLength; i++) {
            vert_move_object(moving + i, brickLength, brick, level, maxLvl, mario, movingLength, moving, score);
            horizon_move_object(moving + i, brickLength, brick, level, maxLvl, mario, movingLength, moving, score);
            
            if (moving[i].y > MAP_HEIGHT) {
                delete_moving(movingLength, moving, i);
                i--;
                continue;
            }
            put_object_on_map(map, MAP_HEIGHT, MAP_WIDTH, moving[i]);
        }
        
        put_object_on_map(map, MAP_HEIGHT, MAP_WIDTH, mario);
        put_score_on_map(map, MAP_WIDTH, score);
        
        set_cur(0, 0);
        show_map(map, MAP_HEIGHT, MAP_WIDTH);
        
        {
            fd_set f; 
            struct timeval t = {0, 10000}; 
            FD_ZERO(&f); 
            FD_SET(0, &f); 
            select(1, &f, 0, 0, &t); 
            char c; 
            if (read(0, &c, 1) == 1 && c == 27) {
                break;
            }
        }
    } while (1);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\033[?25h"); 
    fflush(stdout);
    
    for (int i = 0; i < MAP_HEIGHT; i++) {
        delete[] map[i];
    }
    delete[] map;
    free(brick);
    free(moving);
    
    return 0;
}


void create_level(int &maxLvl, int &brickLength, int &movingLength,
							  GameObject* &brick, GameObject* &moving, 
							  GameObject &mario, int &score, int lvl) {
    maxLvl = 3;
    brickLength = 0;
    movingLength = 0;
    
    free(brick); 
    brick = nullptr;
    free(moving); 
    moving = nullptr;
    
    init_object(&mario, 39, 10, 3, 3, '@');
    score = 0;
    
    switch (lvl) {
        case 1:
            init_object(GetNewBrick(brickLength, brick), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 60, 15, 40, 10, '#');
            init_object(GetNewBrick(brickLength, brick), 100, 20, 20, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 120, 15, 10, 10, '#');
            init_object(GetNewBrick(brickLength, brick), 150, 20, 40, 5, '#');
            
            init_object(GetNewBrick(brickLength, brick), 30, 10, 5, 3, '?');
            init_object(GetNewBrick(brickLength, brick), 50, 10, 5, 3, '?');
            init_object(GetNewBrick(brickLength, brick), 70, 5, 5, 3, '?');
            init_object(GetNewBrick(brickLength, brick), 80, 5, 5, 3, '?');
            
            init_object(GetNewBrick(brickLength, brick), 60, 5, 10, 3, '-');
            init_object(GetNewBrick(brickLength, brick), 75, 5, 5, 3, '-');
            init_object(GetNewBrick(brickLength, brick), 85, 5, 10, 3, '-');
            
            init_object(GetNewBrick(brickLength, brick), 210, 15, 10, 10, '+');
            
            init_object(GetNewMoving(movingLength, moving), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 80, 10, 3, 2, 'o');
            break;
            
        case 2:
            init_object(GetNewBrick(brickLength, brick), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 60, 15, 10, 10, '#');
            init_object(GetNewBrick(brickLength, brick), 80, 20, 20, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 120, 15, 10, 10, '#');
            init_object(GetNewBrick(brickLength, brick), 150, 20, 40, 5, '#');
            
            init_object(GetNewBrick(brickLength, brick), 210, 15, 10, 10, '+');
            
            init_object(GetNewMoving(movingLength, moving), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 80, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 65, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 120, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 160, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 175, 10, 3, 2, 'o');
            break;
            
        case 3:
            init_object(GetNewBrick(brickLength, brick), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 80, 20, 15, 5, '#');
            init_object(GetNewBrick(brickLength, brick), 120, 15, 15, 10, '#');
            
            init_object(GetNewBrick(brickLength, brick), 160, 10, 15, 15, '+');
            
            init_object(GetNewMoving(movingLength, moving), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 50, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 80, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 90, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 120, 10, 3, 2, 'o');
            init_object(GetNewMoving(movingLength, moving), 130, 10, 3, 2, 'o');
            break;
            
        default:
            break;
    }
}


void clear_map(char** map, int MAP_HEIGHT, int MAP_WIDTH) {
    for (int i = 0; i < MAP_WIDTH; i++) {
        map[0][i] = ' ';
    }
    map[0][MAP_WIDTH] = '\0';
    
    for (int j = 0; j < MAP_HEIGHT; j++) {
        strcpy(map[j], map[0]);
    }
}


void delete_moving(int &movingLength, GameObject* &moving, int i) {
    movingLength--;
    moving[i] = moving[movingLength];
    moving = (GameObject*)realloc(moving, sizeof(GameObject) * movingLength);
}


void horizon_move_map(GameObject &mario, int brickLength, GameObject brick[],
											int movingLength, GameObject moving[], float dx) {
												
    mario.x -= dx;
    
    for (int i = 0; i < brickLength; i++) {
        if (is_collision(mario, brick[i])) {
            mario.x += dx;
            return;
        }
    }
    
    mario.x += dx;
    
    for (int i = 0; i < brickLength; i++) { 
        brick[i].x += dx;
    }
    
    for (int i = 0; i < movingLength; i++) {
        moving[i].x += dx;
    }
}


void horizon_move_object(GameObject *obj, int &brickLength, GameObject* &brick,
											   int &level, int &maxLvl, GameObject &mario,
											   int &movingLength, GameObject* &moving, int &score) {
    obj->x += obj->horizSpeed;
    
    for (int i = 0; i < brickLength; i++) {
        if (is_collision(*obj, brick[i])) {
            obj->x -= obj->horizSpeed;
            obj->horizSpeed = -obj->horizSpeed;
            return;
        }
    }
    
    if (obj->cType == 'o') {
        GameObject tmp = *obj;
        vert_move_object(&tmp, brickLength, brick, level, maxLvl, mario, movingLength, moving, score);
        
        if (tmp.IsFly == true) {
            obj->x -= obj->horizSpeed;
            obj->horizSpeed = -obj->horizSpeed;
        }
    }
} 


void init_object(GameObject *obj, float xPos, float yPos,
						   float oWidth, float oHeight, char inType) {
    set_object_pos(obj, xPos, yPos);
    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertSpeed = 0;
    obj->cType = inType;
    obj->horizSpeed = 0.2;
}


bool is_collision(GameObject o1, GameObject o2) {
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
           ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
}


bool is_pos_in_map(int x, int y, int MAP_WIDTH, int MAP_HEIGHT) {
    return (x >= 0) && (x < MAP_WIDTH) && (y >= 0) && (y < MAP_HEIGHT);
}


void mario_collision(GameObject &mario, int &movingLength, GameObject* &moving,
								   int &score, int &level, int &maxLvl, int &brickLength, GameObject* &brick) {
    for (int i = 0; i < movingLength; i++) {
        if (is_collision(mario, moving[i])) {
            if (moving[i].cType == 'o') {
                if (mario.IsFly == true && mario.vertSpeed > 0 && (mario.y + mario.height < moving[i].y + moving[i].height * 0.5)) {
                    score += 50;
                    delete_moving(movingLength, moving, i);
                    i--;
                    continue;
                }
                else {
                    player_dead(level, maxLvl, brickLength, movingLength, brick, moving, mario, score);
                }
            }
    
            if (moving[i].cType == '$') {
                score += 100;
                delete_moving(movingLength, moving, i);
                i--;
                continue;
            }
        }
    }
} 


void player_dead(int &level, int &maxLvl, int &brickLength,
							   int &movingLength, GameObject* &brick, GameObject* &moving,
							   GameObject &mario, int &score) {
    printf("\033[41m\033[2J"); 
    fflush(stdout);
    usleep(500000);
    printf("\033[44m\033[2J"); 
    fflush(stdout);
    
    create_level(maxLvl, brickLength, movingLength, brick, moving, mario, score, level);
}


void put_object_on_map(char** map, int MAP_HEIGHT, int MAP_WIDTH, GameObject obj) {
    int ix = (int)round(obj.x);
    int iy = (int)round(obj.y);
    
    int iWidth = (int)round(obj.width);
    int iHeight = (int)round(obj.height);
    
    for (int i = ix; i < (ix + iWidth); i++) {
        for (int j = iy; j < (iy + iHeight); j++) { 
            if (is_pos_in_map(i, j, MAP_WIDTH, MAP_HEIGHT)) {
                map[j][i] = obj.cType;
            }
        }
    }
}


void put_score_on_map(char** map, int MAP_WIDTH, int score) {
    char c[30];
    sprintf(c, "Score: %d", score);
    int len = strlen(c);
    
    for (int i = 0; i < len; i++) {
        map[0][i+5] = c[i];
    }
}


void set_object_pos(GameObject *obj, float xPos, float yPos) {
    obj->x = xPos;
    obj->y = yPos;
}


void show_map(char** map, int MAP_HEIGHT, int MAP_WIDTH) {
    map[MAP_HEIGHT - 1][MAP_WIDTH - 1] = '\0';
    
    for (int j = 0; j < MAP_HEIGHT; j++) {
        printf("%s", map[j]);
    }
}


void set_cur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}


void vert_move_object(GameObject *obj, int &brickLength, GameObject* &brick,
										int &level, int &maxLvl, GameObject &mario,
										int &movingLength, GameObject* &moving, int &score) {
    obj->IsFly = true;
    obj->vertSpeed += 0.05;
    
    set_object_pos(obj, obj->x, obj->y + obj->vertSpeed);
    
    for (int i = 0; i < brickLength; i++) {
        if (is_collision(*obj, brick[i])) {
            if (obj->vertSpeed > 0) {
                obj->IsFly = false;
            }
            
            if ((brick[i].cType == '?') && (obj->vertSpeed < 0) && (obj == &mario)) {
                brick[i].cType = '-';
                init_object(GetNewMoving(movingLength, moving), brick[i].x, brick[i].y-3, 3, 2, '$');
                moving[movingLength - 1].vertSpeed = -0.7;
            }
            
            obj->y -= obj->vertSpeed;
            obj->vertSpeed = 0;
            obj->IsFly = false;
            
            if (brick[i].cType == '+') {
                level++;
                
                if (level > maxLvl) {
                    level = 1;
                }
                
                printf("\033[42m\033[2J"); 
                fflush(stdout);
                usleep(500000); 
                printf("\033[44m\033[2J"); 
                fflush(stdout); 
                
                create_level(maxLvl, brickLength, movingLength, brick, moving, mario, score, level);
                usleep(10000);
            }
            break;
        }
    }
}


GameObject *GetNewBrick(int &brickLength, GameObject* &brick) {
    brickLength++;
    brick = (GameObject*)realloc(brick, sizeof(GameObject) * brickLength);
    return brick + brickLength - 1;
}


GameObject *GetNewMoving(int &movingLength, GameObject* &moving) {
    movingLength++;
    moving = (GameObject*)realloc(moving, sizeof(GameObject) * movingLength);
    return moving + movingLength - 1;
}
