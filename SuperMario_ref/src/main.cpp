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

void ClearMap();
void CreateLevel(int lvl);
void DeleteMoving(int i);
void HorizonMoveMAp(float dx);
void HorizonMoveObject(TObject *obj);
void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType);
bool IsCollision(TObject o1, TObject o2);
bool IsPosInMap(int x, int y);
void MarioCollision();
void PlayerDead();
void PutObjectOnMap(TObject obj);
void PutScoreOnMap();
void SetObjectPos(TObject *obj, float xPos, float yPos);
void ShowMap();
void setCur(int x, int y);
void VertMoveObject(TObject *obj);

 
void ClearMap() {
    for (int i = 0; i < mapWidth; i++) {
		map[0][i] = ' ';
	}
	
	map[0][mapWidth] = '\0';
	
	for (int j = 0; j < mapHeight; j++) {
		strcpy(map[j], map[0]);
	}
}


void CreateLevel(int lvl) {
	maxLvl = 3;
	brickLength = 0;
	movingLength = 0;
	
	free(brick); brick = nullptr;
	free(moving); moving = nullptr;
	
	InitObject(&mario, 39, 10, 3, 3, '@');
	score = 0;
	
	if (lvl == 1) {
		InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
		InitObject(GetNewBrick(), 60, 15, 40, 10, '#');
		InitObject(GetNewBrick(), 100, 20, 20, 5, '#');
		InitObject(GetNewBrick(), 120, 15, 10, 10, '#');
		InitObject(GetNewBrick(), 150, 20, 40, 5, '#');
		
		InitObject(GetNewBrick(), 30, 10, 5, 3, '?');
		InitObject(GetNewBrick(), 50, 10, 5, 3, '?');
		InitObject(GetNewBrick(), 70, 5, 5, 3, '?');
		InitObject(GetNewBrick(), 80, 5, 5, 3, '?');
		
		InitObject(GetNewBrick(), 60, 5, 10, 3, '-');
		InitObject(GetNewBrick(), 75, 5, 5, 3, '-');
		InitObject(GetNewBrick(), 85, 5, 10, 3, '-');
		
		InitObject(GetNewBrick(), 210, 15, 10, 10, '+');
		
		InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
	}
	
	if (lvl == 2) {
		InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
		InitObject(GetNewBrick(), 60, 15, 10, 10, '#');
		InitObject(GetNewBrick(), 80, 20, 20, 5, '#');
		InitObject(GetNewBrick(), 120, 15, 10, 10, '#');
		InitObject(GetNewBrick(), 150, 20, 40, 5, '#');
		
		InitObject(GetNewBrick(), 210, 15, 10, 10, '+');
		
		InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 65, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 120, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 160, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 175, 10, 3, 2, 'o');
	}
	
	if (lvl == 3) {
		InitObject(GetNewBrick(), 20, 20, 40, 5, '#');
		InitObject(GetNewBrick(), 80, 20, 15, 5, '#');
		InitObject(GetNewBrick(), 120, 15, 15, 10, '#');
		
		InitObject(GetNewBrick(), 160, 10, 15, 15, '+');
		
		InitObject(GetNewMoving(), 25, 10, 3, 2, 'o');
        InitObject(GetNewMoving(), 50, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 80, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 90, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 120, 10, 3, 2, 'o');
		InitObject(GetNewMoving(), 130, 10, 3, 2, 'o');
	}
}


void DeleteMoving(int i) {
	movingLength--;
	moving[i] = moving[movingLength];
	moving = (TObject*)realloc( moving, sizeof(TObject) * movingLength );
}


void HorizonMoveMAp(float dx) {
	mario.x -= dx;
	
	for (int i = 0; i < brickLength; i++) {
		if (IsCollision(mario, brick[i])) {
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


void HorizonMoveObject(TObject *obj) {
	obj[0].x += obj[0].horizSpeed;
	
	for (int i = 0; i < brickLength; i++) {
		if (IsCollision(obj[0], brick[i])) {
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
			return;
		}
	}
	
	if (obj[0].cType == 'o'){
		TObject tmp = *obj;
		VertMoveObject(&tmp);
		
		if (tmp.IsFly == true) {
			obj[0].x -= obj[0].horizSpeed;
			obj[0].horizSpeed = -obj[0].horizSpeed;
		}
	}
} 


void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {
	SetObjectPos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
	(*obj).vertSpeed = 0;
	(*obj).cType = inType;
	(*obj).horizSpeed = 0.2;
}


bool IsCollision(TObject o1, TObject o2) {
	return (( o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
				((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height)) ;
}


bool IsPosInMap(int x, int y) {
	return ( (x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight) );
}


void MarioCollision() {
	for (int i = 0; i < movingLength; i++){
		if (IsCollision( mario, moving[i])) {
			if (moving[i].cType == 'o') {
				if (       (mario.IsFly == true)
					&&  (mario.vertSpeed > 0)
					&&  (  mario.y + mario.height < moving[i].y + moving[i].height * 0.5 )
					)
					{
						score += 50;
						DeleteMoving(i);
						i--;
						continue;
					}
				else
						PlayerDead();
			}
	
			if  (moving[i].cType == '$') {
				score += 100;
				DeleteMoving(i);
				i--;
				continue;
			}
		}
	}
} 


void PlayerDead() {
    printf("\033[41m\033[2J"); fflush(stdout);
    usleep(500000);
    printf("\033[44m\033[2J"); fflush(stdout);
	
    CreateLevel(level);
}


void PutObjectOnMap(TObject obj) {
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	
	int iWidth = (int)round(obj.width);
	int iHeight = (int)round(obj.height);
	
	for (int i = ix; i < (ix + iWidth); i++) {
		for (int j = iy; j < (iy + iHeight); j++) { 
			if (IsPosInMap(i, j)) {
				map[j][i] = obj.cType;
			}
		}
	}
}


void PutScoreOnMap() {
	char c[30];
	sprintf(c, "Score: %d", score);
	int len = strlen(c);
	
	for (int i = 0; i < len; i++) {
		map[0][i+5] = c[i];
	}
}


void SetObjectPos(TObject *obj, float xPos, float yPos) {
	(*obj).x = xPos;
	(*obj).y = yPos;
}


void ShowMap() {
	map[mapHeight - 1][mapWidth - 1] = '\0';
	
	for (int j = 0; j < mapHeight; j++) {
		printf("%s", map[j]);
	}
}


void setCur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}


void VertMoveObject(TObject *obj) {
	(*obj).IsFly = true;
	(*obj).vertSpeed += 0.05;
	
	SetObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);
	
	for (int i = 0; i < brickLength; i++){
		
		if (IsCollision( *obj, brick[i])) {
			if (obj[0].vertSpeed > 0) {
				obj[0].IsFly = false;
			}
			
			if ( (brick[i].cType == '?') && (obj[0].vertSpeed < 0) && (obj == &mario)) {
				brick[i].cType = '-';
				InitObject(GetNewMoving(), brick[i].x, brick[i].y-3, 3, 2, '$');
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
				
				CreateLevel(level);
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
	CreateLevel(level);
	
	struct termios oldt, newt; tcgetattr(STDIN_FILENO,&oldt); newt=oldt; newt.c_lflag&=~(ICANON|ECHO); newt.c_cc[VMIN]=0; newt.c_cc[VTIME]=0; tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	printf("\033[44m\033[2J");
	printf("\033[?25l"); fflush(stdout);
	
	do {
		ClearMap();
		
		{ fd_set f; struct timeval t={0,16000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1) { if(c==27) break; if(c==' ') mario.vertSpeed=-1; if(c=='a'||c=='A') HorizonMoveMAp(2); if(c=='d'||c=='D') HorizonMoveMAp(-2); } }
		
		if (mario.y > mapHeight) {
			PlayerDead();
		}
		
		VertMoveObject(&mario);
		MarioCollision();
		
		for (int i = 0; i < brickLength; i++) {
			PutObjectOnMap(brick[i]);
		}
		
		for (int i = 0; i < movingLength; i++) {
			VertMoveObject(moving + i);
			HorizonMoveObject(moving + i);
			if (moving[i].y > mapHeight) {
				DeleteMoving(i);
				i--;
				continue;
			}
			PutObjectOnMap(moving[i]);
		}
		
		PutObjectOnMap(mario);
		PutScoreOnMap();
		
		setCur(0, 0);
		ShowMap();
		{ fd_set f; struct timeval t={0,10000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1 && c==27) break; }
	} while (1);
	
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	printf("\033[?25h"); fflush(stdout);
	return 0;
}
