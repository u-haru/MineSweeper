#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define Cell_x 60
#define Cell_y 30
#define Bomb_num 10
//マス目の10分の1が爆弾になる
//#define Bomb_num 2//とすると爆弾の数が2になる

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void Init();
void SetBomb(u_int x,u_int y);
void LClick(u_int x,u_int y);
void RClick(u_int x,u_int y);
void OpenCell(u_int x,u_int y);

struct _cell {
	bool opened;
	bool bomb;
	bool flag;
	u_int num;
};