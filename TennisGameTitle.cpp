#include	"TennisInclude.h"
#include	<stdlib.h>
#include	<math.h>

static void Title(int* scene);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetWindowText("テニスゲーム");
	SetGraphMode(WIDTH, HEIGHT, 32);;
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == 1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	int scene = TITLE;
	Circle circle = { WIDTH / 2,HEIGHT / 2,30,5,5 };
	Box box = { WIDTH / 2,HEIGHT - 80,150,20,10 };

	while (1) {
		ClearDrawScreen();

		switch (scene) {
		case TITLE:
			Title(&scene);
			break;
		case GAME:
			Game(&scene, &circle, &box,time(NULL));
			break;
		case GAMEOVER:
			break;
		}
		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();
	return 0;
}

static void Title(int* scene) {
	SetFontSize(150);
	DrawString(WIDTH / 2 - 150 * 3 + 150 / 5 / 4, HEIGHT / 2 - 150 * 2, "Tennis Game", OLANGE);
	SetFontSize(60);
	if(time(NULL) % 2 == 0) DrawString(WIDTH / 2 - 60 * 5 - 60 / 3, HEIGHT / 2 + 60 * 3, "Press SPACE to start", WHITE);

	if (CheckHitKey(KEY_INPUT_SPACE)) *scene = GAME;
}