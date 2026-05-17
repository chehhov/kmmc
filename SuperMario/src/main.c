#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 


#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#define mapWidth 80
#define mapHeight 25


typedef struct SObject {
	float x, y;
	float width, height;
	float vertSpeed;
	bool IsFly;
	char cType;
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;
TObject *brick = NULL;
int brickLength;

void ClearMap() {
    for (int i = 0; i < mapWidth; i++)
		map[0][i] = ' ';
	map[0][mapWidth] = '\0';
	for (int j = 0; j < mapHeight; j++)
		sprintf( map[j], map[0]);
}

void ShowMap() {
	map[mapHeight - 1][mapWidth - 1] = '\0';
	for (int j = 0; j < mapHeight; j++)
		printf("%s", map[j]);
}

void SetObjectPos(TObject *obj, float xPos, float yPos) {
	(*obj).x = xPos;
	(*obj).y = yPos;
}

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight, char inType) {
	SetObjectPos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
	(*obj).vertSpeed = 0;
	(*obj).cType = inType;
}

bool IsCollision(TObject o1, TObject o2);

void CreateLevel();

void VertMoveObject(TObject *obj) {
	(*obj).IsFly = true;
	(*obj).vertSpeed += 0.05;
	SetObjectPos(obj, (*obj).x, (*obj).y + (*obj).vertSpeed);
	
	for (int i = 0; i < brickLength; i++)
		if (IsCollision( *obj, brick[i])) {
			(*obj).y -= (*obj).vertSpeed;
			(*obj).vertSpeed = 0;
			(*obj).IsFly = false;
			if (brick[i].cType == '+') {
				CreateLevel();
				usleep(10000);
			}
			break;
		}
}

bool IsPosInMap(int x, int y) {
	return ( (x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight) );
}

void PutObjectOnMap(TObject obj) {
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	int iWidth = (int)round(obj.width);
	int iHeight = (int)round(obj.height);
	
	for (int i = ix; i < (ix + iWidth); i++) 
		for (int j = iy; j < (iy + iHeight); j++) 
			if (IsPosInMap(i, j))
				map[j][i] = obj.cType;
}

void setCur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void HorizonMoveMAp(float dx) {
	mario.x -= dx;
	for (int i = 0; i < brickLength; i++) 
		if (IsCollision(mario, brick[i])) {
			mario.x += dx;
			return;
			
		}
	mario.x += dx;
	
	for (int i = 0; i < brickLength; i++) 
		brick[i].x += dx;
}

bool IsCollision(TObject o1, TObject o2) {
	return (( o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
				((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height)) ;
}

void CreateLevel() {
	InitObject(&mario, 39, 10, 3, 3, '@');
	
	brickLength = 6;
	brick = realloc(brick,  sizeof(*brick) * brickLength );
	InitObject(brick+0, 20, 20, 40, 5, '#');
	InitObject(brick+1, 60, 15, 10, 10, '#');
	InitObject(brick+2, 80, 20, 20, 5, '#');
	InitObject(brick+3, 120, 15, 10, 10, '#');
	InitObject(brick+4, 150, 20, 40, 5, '#');
	InitObject(brick+5, 210, 15, 10, 10, '+');
}

int main() {
	CreateLevel();
	struct termios oldt, newt; tcgetattr(STDIN_FILENO,&oldt); newt=oldt; newt.c_lflag&=~(ICANON|ECHO); newt.c_cc[VMIN]=0; newt.c_cc[VTIME]=0; tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	do {
		ClearMap();
		
		{ fd_set f; struct timeval t={0,1000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1) { if(c==27) break; if(c==' ') mario.vertSpeed=-0.7; if(c=='a'||c=='A') HorizonMoveMAp(1); if(c=='d'||c=='D') HorizonMoveMAp(-1); } }
		
		if (mario.y > mapHeight) CreateLevel();
		
		VertMoveObject(&mario);
		for (int i = 0; i < brickLength; i++)
			PutObjectOnMap(brick[i]);
		PutObjectOnMap(mario);
		
		setCur(0, 0);
		ShowMap();
		usleep(10000);
		{ fd_set f; struct timeval t={0,10000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1 && c==27) break; }
	} while (1);
	
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return 0;
}
