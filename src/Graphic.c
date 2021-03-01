#include "Graphic.h"
#include "main.h"

HDC hMdc;
HWND ghWnd = NULL;
extern struct _cell Cell[Cell_x][Cell_y];

void DrawCell(u_int x,u_int y,HGDIOBJ h){
	char num[2];
	itoa(Cell[x][y].num,num,10);
	SelectObject(hMdc , h);//ブラシ選択
	Rectangle(hMdc,30*x,30*y,30*(x+1),30*(y+1));
	if(Cell[x][y].num != 0 && !Cell[x][y].bomb){
		TextOut(hMdc, x*30, y*30, num, 1);//周りの爆弾の数
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc = NULL;
	PAINTSTRUCT ps;
	switch (msg) {
		case WM_KEYDOWN:{
            switch(wp)
            {   case VK_ESCAPE:
					Init();
					InvalidateRect(ghWnd, NULL, TRUE);
            }
            break;
		}
		case WM_LBUTTONUP:{
			u_int x,y;
			x = LOWORD(lp)/30;
			y = HIWORD(lp)/30;
			LClick(x,y);
			InvalidateRect(ghWnd, NULL, TRUE);  //領域無効化
			break;
		}
		case WM_RBUTTONUP:{
			u_int x,y;
			x = LOWORD(lp)/30;
			y = HIWORD(lp)/30;
			RClick(x,y);
			InvalidateRect(ghWnd, NULL, TRUE);
			break;
		}
		case WM_CREATE:{
			srand((unsigned)time(NULL));
			hdc = GetDC(hWnd);

			HBITMAP hMBit=CreateCompatibleBitmap(hdc,Cell_x*30+5,Cell_x*30+28);
			hMdc = CreateCompatibleDC(NULL);
			SelectObject(hMdc,hMBit);
			DeleteObject(hMBit);
			InvalidateRect(ghWnd, NULL, TRUE);
			UpdateWindow(ghWnd);
			break;
		}
		case WM_DESTROY:
			DestroyWindow(ghWnd);
			PostQuitMessage(0);
			DeleteDC(hdc);
			DeleteDC(hMdc);
			return 0;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			BitBlt(hdc, 0, 0, Cell_x*30+5, Cell_x*30+28, hMdc, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			break;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}