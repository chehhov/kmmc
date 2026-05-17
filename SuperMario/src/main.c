#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#define mapWidth 80
#define mapHeight 25


typedef struct SObject {
	float x, y;
	float width, height;
} TObject;

char map[mapHeight][mapWidth+1];
TObject mario;

void ClearMap() {
    for (int i = 0; i < mapWidth; i++)
		map[0][i] = '.';
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

void InitObject(TObject *obj, float xPos, float yPos, float oWidth, float oHeight) {
	SetObjectPos(obj, xPos, yPos);
	(*obj).width = oWidth;
	(*obj).height = oHeight;
}

void PutObjectOnMap(TObject obj) {
	int ix = (int)round(obj.x);
	int iy = (int)round(obj.y);
	int iWidth = (int)round(obj.width);
	int iHeight = (int)round(obj.height);
	
	for (int i = ix; i < (ix + iWidth); i++) 
		for (int j = iy; j < (iy + iHeight); j++) 
			map[j][i] = '@';
}

void setCur(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

int main() {
	InitObject(&mario, 39, 10, 3, 3);
	
	struct termios oldt, newt; tcgetattr(STDIN_FILENO,&oldt); newt=oldt; newt.c_lflag&=~(ICANON|ECHO); newt.c_cc[VMIN]=0; newt.c_cc[VTIME]=0; tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	do {
		ClearMap();
		PutObjectOnMap(mario);
		
		setCur(0, 0);
		ShowMap();
		{ fd_set f; struct timeval t={0,10000}; FD_ZERO(&f); FD_SET(0,&f); select(1,&f,0,0,&t); char c; if(read(0,&c,1)==1 && c==27) break; }
	} while (1);
	
	tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	return 0;
}