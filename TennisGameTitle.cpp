#include	"TennisInclude.h"

static bool ErrorCheck(int* bgm, const int bgmSize);
static void Title(int* scene);
static void GameOver(int* scene,const time_t startTimer, const time_t nowTime);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetWindowText("テニスゲーム");
	SetGraphMode(WIDTH, HEIGHT, 32);;
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == 1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	int scene = TITLE;

	int img = LoadGraph("image/bg.png");

	int bgm[] = {
		LoadSoundMem("sound/bgm.mp3"),
		LoadSoundMem("sound/gameover.mp3"),
		LoadSoundMem("sound/hit.mp3"),

	};
	const int bgmSize = sizeof bgm / sizeof bgm[0];

	Circle circle = { WIDTH / 2,HEIGHT / 2,30,5,-5 };
	Circle resetCircle = circle;

	PlayerBox box = { WIDTH / 2,HEIGHT - 80,150,20,10 };
	PlayerBox resetBox = box;

	bool reset = false;

	static time_t startTimer = time(NULL);

	while (1) {
		if (img == -1) break;
		if (ErrorCheck(bgm, bgmSize)) break;

		ClearDrawScreen();

		switch (scene) {
		case TITLE:
			Title(&scene);
			break;
		case GAME:
			Game(&scene,img,bgm,&reset,&circle,&resetCircle,&box,&resetBox,time(NULL));
			break;
		case GAMEOVER:
			if (reset) {
				startTimer = time(NULL);
				reset = false;
			}
			GameOver(&scene,startTimer,time(NULL));
			break;
		}
		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DeleteGraph(img);
	for (int i = 0; i < bgmSize; i++) DeleteSoundMem(bgm[i]);

	DxLib_End();
	return 0;
}

static bool ErrorCheck(int* bgm,const int bgmSize) {
	for (int i = 0; i < bgmSize; i++) if (bgm[i] == -1) return true;
	return false;
}

static void Title(int* scene) {
	SetFontSize(120);
	DrawString(WIDTH / 2 - 120 * 3 + 120 / 12, HEIGHT / 2 - 120 * 2, "Tennis Game", OLANGE);
	SetFontSize(40);
	if(time(NULL) % 2 == 0) DrawString(WIDTH / 2 - 40 * 5 - 40 / 4, HEIGHT / 2 + 60 * 3, "Press SPACE to start", WHITE);

	if (CheckHitKey(KEY_INPUT_SPACE)) *scene = GAME;
}

static void GameOver(int* scene,const time_t startTimer, const time_t nowTime) {
	double explasedTime = (double)nowTime - startTimer;

	SetFontSize(100);
	DrawString(WIDTH / 2 - 100 * 2 - 50, HEIGHT / 2 - 50, "GAME OVER", RED);
	if (explasedTime >= 5) *scene = TITLE;
}