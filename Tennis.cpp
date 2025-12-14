#include	"TennisInclude.h"

static bool Move(Circle* circle, Box* box);
static int Judge(bool* bounce,const Circle* circle, const Box* box);
static void Draw(const int& score,const int& highScore,const Circle* circle, const Box* box);

void Game(int* scene,Circle* circle,Box* box,time_t nowTime) {
	static int score = 0;
	static int highScore = 0;

	static time_t startTime = time(NULL);

	double explasedTime = (double)(nowTime - startTime);

	static bool bounce = false;

	if (!Move(circle, box)) {
		*scene = GAMEOVER;
	}
	else {
		int judge = Judge(&bounce,circle, box);

		if (!bounce && judge == 2) {
			circle->circleScalar.x = -circle->circleScalar.x;
			circle->circleScalar.y = -circle->circleScalar.y;
			bounce = true;

			score += 100;
			if (score > highScore) highScore = score;
		}
		else if (!bounce && judge == 1) {
			circle->circleScalar.y = -circle->circleScalar.y;
			bounce = true;

			score += 100;
			if (score > highScore) highScore = score;
		}
		else if (!bounce && judge == 0) {
			circle->circleScalar.x = -circle->circleScalar.x;
			bounce = true;
		}

		Draw(score, highScore, circle, box);
	}
}

static bool Move(Circle* circle, Box* box) {
	//円の動きの処理
	circle->pos.x += circle->circleScalar.x;
	circle->pos.y += circle->circleScalar.y;

	//円と壁との判定の処理(ただし、スクリーンのy軸の最高座標まで行った場合はゲームオーバーへ移る)
	if (circle->pos.x <= circle->radius && circle->circleScalar.x < 0) circle->circleScalar.x = -circle->circleScalar.x;
	if (circle->pos.x >= WIDTH - circle->radius && circle->circleScalar.x > 0) circle->circleScalar.x = -circle->circleScalar.x;
	if (circle->pos.y <= circle->radius && circle->circleScalar.y < 0) circle->circleScalar.y = -circle->circleScalar.y;
	if (circle->pos.y >= HEIGHT - circle->radius) return false;

	//プレイヤーが操作をする時の矩形の動きの処理
	if (box->pos.x > box->length.x / 2 && (CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_LEFT))) {
		box->pos.x -= box->boxScalar;
		if (box->pos.x < box->length.x / 2) box->pos.x = box->length.x / 2;
	}
	if (box->pos.x < WIDTH - box->length.x / 2 && (CheckHitKey(KEY_INPUT_D) || CheckHitKey(KEY_INPUT_RIGHT))) {
		box->pos.x += box->boxScalar;
		if (box->pos.x > WIDTH - box->length.x / 2) box->pos.x = WIDTH - box->length.x / 2;
	}

	return true;
}

static int Judge(bool* bounce,const Circle* circle,const Box* box) {
	int dX = abs(circle->pos.x - box->pos.x);
	int dY = abs(circle->pos.y - box->pos.y);
	int rangeX = circle->radius + box->length.x / 2;
	int rangeY = circle->radius + box->length.y / 2;

	if ((dX <= rangeX && dX > box->length.x / 2) && (dY <= rangeY && dY > box->length.y / 2)) {
		return 2;
	}
	else if((dX <= rangeX && dX <= box->length.x / 2) && (dY <= rangeY && dY > box->length.y / 2)){
		return 1;
	}
	else if(dX <= rangeX && (dY <= rangeY && dY <= box->length.y / 2)) {
		return 0;
	}
	else {
		*bounce = false;
		return -1;
	}
}

static void Draw(const int& score, const int& highScore, const Circle* circle, const Box* box) {
	DrawCircle(circle->pos.x, circle->pos.y, circle->radius, 0xff0000, TRUE);
	DrawBox(box->pos.x - box->length.x / 2, box->pos.y - box->length.y / 2, box->pos.x + box->length.x / 2, box->pos.y + box->length.y / 2, 0x0080ff, TRUE);

	SetFontSize(30);
	DrawFormatString(30, 30, WHITE, "Score:%d点", score);
	DrawFormatString(WIDTH - 270, 30, YELLOW, "HighScore:%d点", highScore);
}