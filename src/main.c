#include "main.h"
#include "Graphic.h"

struct _cell Cell[Cell_x][Cell_y];
u_int openedCell;

void Init(){
	SetWindowText(ghWnd , "MineSweeper");
	SelectObject(hMdc , GetStockObject(WHITE_BRUSH));//白色を選択
	for(u_int i = 0;i < Cell_x;i++){
		for(u_int j = 0;j < Cell_y;j++){
			Rectangle(hMdc,30*i,30*j,30*(i+1),30*(j+1));//番号描画回避のため、DrawCellは使えない
			Cell[i][j].num=0;
			Cell[i][j].opened = false;
			Cell[i][j].bomb = false;
			Cell[i][j].flag = false;
		}
	}
	for(u_int i = 0;i < Bomb_num;i++){//爆弾設置
		u_int x = rand()%Cell_x;
		u_int y = rand()%Cell_y;
		if(!Cell[x][y].bomb)
		SetBomb(x,y);
	}
	openedCell = 0;
}
void SetBomb(u_int x,u_int y){
	Cell[x][y].bomb = true;
	for(int i = -1;i<=1;i++){//周りの番号加算
		for(int j = -1;j<=1;j++){
			if(x+i < 0|| y+j < 0 || x+i >= Cell_x || y+j >= Cell_y) continue;
			Cell[x+i][y+j].num++;
		}
	}
}

void OpenAll(){//答え合わせ
	for(u_int i = 0;i < Cell_x;i++){
		for(u_int j = 0;j < Cell_y;j++){
			if(Cell[i][j].bomb){
				DrawCell(i,j,GetStockObject(BLACK_BRUSH));
			}else{
				DrawCell(i,j,GetStockObject(GRAY_BRUSH));
			}
			// DeleteObject(SelectObject(hMdc , GetStockObject(WHITE_BRUSH)));
		}
	}
}

void LClick(u_int x,u_int y){//空ける
	OpenCell(x,y);
}

void RClick(u_int x,u_int y){//旗立てる
	DrawCell(x,y,CreateSolidBrush(0XFF << 16));//青色(旗の色)で描画
}

void OpenCell(u_int x,u_int y){
	char num[2];
	itoa(Cell[x][y].num,num,10);
	if(Cell[x][y].bomb){
		SetWindowText(ghWnd , "Game Over!!!(Escキーでリセット)");
		OpenAll();
		return;
	}
	DrawCell(x,y,GetStockObject(GRAY_BRUSH));//灰色で描画(空けたマスの色)
	if(Cell[x][y].num != 0 && !Cell[x][y].bomb){
		TextOut(hMdc, x*30, y*30, num, 1);
	}
	// DeleteObject(SelectObject(hMdc , GetStockObject(WHITE_BRUSH)));

	if(Cell[x][y].opened){
		return;
	}
	Cell[x][y].opened = true;

	if(Cell[x][y].num == 0){//番号0なら
		for(int i = -1;i<=1;i++){//周りのマス開ける
			for(int j = -1;j<=1;j++){
				if(x+i < 0 || y+j < 0)continue;
				if(x == 0 && y == 0)continue;
				if(x+i < Cell_x && y+j < Cell_y) OpenCell(x+i,y+j);
			}
		}
	}
	openedCell++;
	if(openedCell == Cell_x * Cell_y - Bomb_num){
		SetWindowText(ghWnd , "Game Clear!!!(Escキーでリセット)");
		OpenAll();
	}
	return;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	MSG msg;
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance , TEXT("NOTE"));
	wc.hCursor = LoadCursor(NULL , IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("Application");

	if (!RegisterClass(&wc)) return 0;
	ghWnd = CreateWindow(
		"Application", "MineSweeper",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		100, 50,
		Cell_x*30+5, Cell_y*30+28,
		NULL, NULL,
		hInstance, NULL);
	Init();
	ShowWindow(ghWnd, nCmdShow);
	UpdateWindow(ghWnd);
	SetFocus(ghWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}