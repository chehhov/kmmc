#include <cstdio>
#include <cstdlib>

#include <cmath>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>


struct TObject {
    float x, y;
    float width, height;
    float vertSpeed;
    bool IsFly;
    char cType;
    float horizSpeed;
};

const int mapWidth = 80;
const int mapHeight = 25;

char map[mapHeight][mapWidth+1];

int brickLength;
int movingLength;


int level = 1;
int score;
int maxLvl;

TObject *GetNewBrick();
TObject *GetNewMoving();

TObject mario;

TObject *brick = nullptr;
TObject *moving = nullptr;

void clear_map();
void create_level(int lvl);
void delete_moving(int i);
void horizon_move_map(float dx);
void horizon_move_object(TObject *obj);
void init_object(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
bool is_collision(TObject o1, TObject o2);
bool is_pos_in_map(int x, int y);
void mario_collision();
void player_dead();
void put_object_on_map(TObject obj);
void put_score_on_map();
void set_object_pos(TObject *obj, float xPos, float yPos);
void show_map();
void set_cur(int x, int y);
void vert_move_object(TObject *obj);

 
void create_level(int lvl) {
    maxLvl = 3;
    brickLength = 0;
    movingLength = 0;
    
    free(brick); brick = nullptr;
    free(moving); moving = nullptr;
    
    init_object(&mario, 39, 10, 3, 3, '@');
    score = 0;
    
    switch (lvl) {
        case 1:
            init_object(GetNewBrick(), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(), 60, 15, 40, 10, '#');
            init_object(GetNewBrick(), 100, 20, 20, 5, '#');
            init_object(GetNewBrick(), 120, 15, 10, 10, '#');
            init_object(GetNewBrick(), 150, 20, 40, 5, '#');
            
            init_object(GetNewBrick(), 30, 10, 5, 3, '?');
            init_object(GetNewBrick(), 50, 10, 5, 3, '?');
            init_object(GetNewBrick(), 70, 5, 5, 3, '?');
            init_object(GetNewBrick(), 80, 5, 5, 3, '?');
            
            init_object(GetNewBrick(), 60, 5, 10, 3, '-');
            init_object(GetNewBrick(), 75, 5, 5, 3, '-');
            init_object(GetNewBrick(), 85, 5, 10, 3, '-');
            
            init_object(GetNewBrick(), 210, 15, 10, 10, '+');
            
            init_object(GetNewMoving(), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 80, 10, 3, 2, 'o');
            break;
            
        case 2:
            init_object(GetNewBrick(), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(), 60, 15, 10, 10, '#');
            init_object(GetNewBrick(), 80, 20, 20, 5, '#');
            init_object(GetNewBrick(), 120, 15, 10, 10, '#');
            init_object(GetNewBrick(), 150, 20, 40, 5, '#');
            
            init_object(GetNewBrick(), 210, 15, 10, 10, '+');
            
            init_object(GetNewMoving(), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 80, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 65, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 120, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 160, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 175, 10, 3, 2, 'o');
            break;
            
        case 3:
            init_object(GetNewBrick(), 20, 20, 40, 5, '#');
            init_object(GetNewBrick(), 80, 20, 15, 5, '#');
            init_object(GetNewBrick(), 120, 15, 15, 10, '#');
            
            init_object(GetNewBrick(), 160, 10, 15, 15, '+');
            
            init_object(GetNewMoving(), 25, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 50, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 80, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 90, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 120, 10, 3, 2, 'o');
            init_object(GetNewMoving(), 130, 10, 3, 2, 'o');
            break;
            
        default:
            break;
    }
}


void clearmap() {
    for (int i = 0; i < mapWidth; i++)
		map[0][i] = ' ';
	map[0][mapWidth] = '\0';
	for (int j = 0; j < mapHeight; j++)
		strcpy(map[j], map[0]);
}

void delete_moving(int i) {
	movingLength--;
	moving[i] = moving[movingLength];
	moving = (TObject*)realloc( moving, sizeof(TObject) * movingLength );
}


void horizon_move_map(float dx) {
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
;;

void horizon_move_object(TObject *obj) {
	obj[0].x += obj[0].horizSpeed;
	
	for (int i = 0; i < brickLength; i++) {
		if (is_collision(obj[0], brick[i])) {
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
			return;
		}
	}
	
	if (obj[0].cType == 'o'){
		TObject tmp = *obj;
		vert_move_object(&tmp);
		
		if (tmp.IsFly == true) {
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
		}
	}
} 


void init_object(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {
	set_object_pos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
	(*obj).vertSpeed = 0;
	(*obj).cType = inType;
	(*obj).horizSpeed = 0.2;
}


bool is_collision(TObject o1, TObject o2) {
	return (( o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
				((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height)) ;
}


bool is_pos_in_map(int x, int y) {
	return ( (x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight) );
}


void mario_collision() {
	for (int i = 0; i < movingLength; i++){
		if (is_collision( mario, moving[i])) {
			if (moving[i].cType == 'o') {
				if (       (mario.IsFly == true)
					&&  (mario.vertSpeed > 0)
					&&  (  mario.y + mario.height < moving[i].y + moving[i].height * 0.5 )
					)
					{
						score += 50;
						delete_moving(i);
						i--;
						continue;
					}
				else
						player_dead();
			}
	
			if  (moving[i].cType == '$') {
				score += 100;
				delete_moving(i);
				i--;
				continue;
			}
		}
	}
} 


void player_dead() {
    printf("\033[41m\033[2J"); fflush(stdout);
    usleep(500000);
    printf("\033[44m\033[2J"); fflush(stdout);
	
    create_level(level);
}


void put_object_on_map(TObject obj) {
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	
	int iWidth = (int)round(obj.width);
	int iHeight = (int)round(obj.height);
	
	for (int i = ix; i < (ix + iWidth); i++) {
		for (int j = iy; j < (iy + iHeight); j++) { 
			if (is_pos_in_map(i, j)) {
				map[j][i] = obj.cType;
			}
		}
	}
}


void put_score_on_map() {
	char c[30];
	sprintf(c, "Score: %d", score);
	int len = strlen(c);
	
	for (int i = 0; i < len; i++) {
		map[0][i+5] = c[i];
	}
}


void set_object_pos(TObject *obj, float xPos, float yPos) {
	(*obj).x = xPos;
	(*obj).y = yPos;
}


void show_map() {
	map[mapHeight - 1][mapWidth - 1] = '\0';
	
	for (int j = 0; j < mapHeight; j++) {
		printf("%s", map[j]);
	}
}


void set_cur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}


void vert_move_object(TObject *obj) {
	(*obj).IsFly = true;
	(*obj).vertSpeed += 0.05;
	
	set_object_pos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);
	
	for (int i = 0; i < brickLength; i++){
		
		if (is_collision( *obj, brick[i])) {
			if (obj[0].vertSpeed > 0) {
				obj[0].IsFly = false;
			}
			
			if ( (brick[i].cType == '?') && (obj[0].vertSpeed < 0) && (obj == &mario)) {
				brick[i].cType = '-';
				init_object(GetNewMoving(), brick[i].x, brick[i].y-3, 3, 2, '$');
				moving[movingLength - 1].vertSpeed = -0.7;
			}
			
			(*obj).y -= (*obj).vertSpeed;
			(*obj).vertSpeed = 0;
			(*obj).IsFly = false;
			
			if (brick[i].cType == '+') {
				level++;
				
				if (level > maxLvl) {
					level = 1;
				}
				
				printf("\033[42m\033[2J"); fflush(stdout);
				usleep(500000); 
				printf("\033[44m\033[2J"); fflush(stdout); 
				
				create_level(level);
				usleep(10000);
			}
			break;
		}
	}
}


TObject *GetNewBrick() {
	brickLength++;
	brick = (TObject*)realloc( brick, sizeof(TObject) * brickLength);
	return brick + brickLength - 1;
}


TObject *GetNewMoving() {
	movingLength++;
	moving = (TObject*)realloc( moving, sizeof(TObject) * movingLength);
	return moving + movingLength - 1;
}


int main() {
	create_level(level);
	
	struct termios oldt, newt; tcgetattr(STDIN_FILENO,&oldt); newt=oldt; newt.c_lflag&=~(ICANON|ECHO); newt.c_cc[VMIN]=0; newt.c_cc[VTIME]=0; tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	printf("\033[44m\033[2J");
	printf("\033[?25l"); fflush(stdout);
	
	do {
		clearmap();
		
		{ fd_set f; struct timeval t={0,16000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1) { if(c==27) break; if(c==' ') mario.vertSpeed=-1; if(c=='a'||c=='A') horizon_move_map(2); if(c=='d'||c=='D') horizon_move_map(-2); } }
		
		if (mario.y > mapHeight) {
			player_dead();
		}
		
		vert_move_object(&mario);
		mario_collision();
		
		for (int i = 0; i < brickLength; i++) {
			put_object_on_map(brick[i]);
		}
		
		for (int i = 0; i < movingLength; i++) {
			vert_move_object(moving + i);
			horizon_move_object(moving + i);
			if (moving[i].y > mapHeight) {
				delete_moving(i);
				i--;
				continue;
			}
			put_object_on_map(moving[i]);
		}
		
		put_object_on_map(mario);
		put_score_on_map();
		
		set_cur(0, 0);
		show_map();
		{ fd_set f; struct timeval t={0,10000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1 && c==27) break; }
	} while (1);
	
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	printf("\033[?25h"); fflush(stdout);
	return 0;
}
