#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

extern HDC hMdc;
extern HWND ghWnd;

void DrawCell(u_int x,u_int y,HGDIOBJ h);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);