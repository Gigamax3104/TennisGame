#include	"TennisInclude.h"
#include	<stdlib.h>

static bool Move(Circle* circle, PlayerBox* box);
static int Judge(bool* bounce,const Circle* circle, const PlayerBox* box);
static void Draw(const int score,const int highScore,const int img,const Circle* circle, const PlayerBox* box);
static void Reset(int* score, Circle* circle, const Circle* resetCircle, PlayerBox* box, const PlayerBox* resetBox);

void Game(int* scene,const int img,const int* bgm,bool* reset,Circle* circle,const Circle* resetCircle, PlayerBox* box,const PlayerBox* resetBox,time_t nowTime) {
	//スコア用変数
	static int score = 0;
	static int highScore = 0;

	//一度のみの跳ね返りを制御する変数
	static bool bounce = false;

	//bgmを再生するための変数
	static bool playSound = true;

	//bgm再生
	if (playSound) {
		ChangeVolumeSoundMem(128, bgm[0]);
		PlaySoundMem(bgm[0], DX_PLAYTYPE_LOOP);
		playSound = false;
	}

	//もし、円が画面の一番下まで行ってしまった場合は、GAME OVERとなる。(その際に、音楽を止め、変数をリセットする。)
	if (!Move(circle, box)) { // 73行目～95行目
		StopSoundMem(bgm[0]);
		PlaySoundMem(bgm[1], DX_PLAYTYPE_BACK);

		Reset(&score, circle, resetCircle, box, resetBox); // 139行目～147行目

		*reset = true;
		playSound = true;
		*scene = GAMEOVER;
	}
	//一番下以外では矩形と円の当たり判定を調べる。(詳細はJudge関数にて掲載)
	else {
		int judge = Judge(&bounce,circle, box); // 97行目～121行目

		if (!bounce && judge == 2) {
			PlaySoundMem(bgm[2], DX_PLAYTYPE_BACK);

			circle->circleScalar.x = -resetCircle->circleScalar.x - (rand() % resetCircle->circleScalar.x);
			circle->circleScalar.y = resetCircle->circleScalar.y - (rand() % resetCircle->circleScalar.y);
			bounce = true;

			score += 100;
			if (score > highScore) highScore = score;
		}
		else if (!bounce && judge == 1) {
			PlaySoundMem(bgm[2], DX_PLAYTYPE_BACK);

			circle->circleScalar.y = resetCircle->circleScalar.y - (rand() % resetCircle->circleScalar.y);
			bounce = true;

			score += 100;
			if (score > highScore) highScore = score;
		}
		else if (!bounce && judge == 0) {
			PlaySoundMem(bgm[2], DX_PLAYTYPE_BACK);

			circle->circleScalar.x = -resetCircle->circleScalar.x - (rand() % resetCircle->circleScalar.x);
			bounce = true;
		}

		//描画する関数(123行目～141行目)
		Draw(score, highScore,img,circle, box); 
	}
}

static bool Move(Circle* circle, PlayerBox* box) {
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

static int Judge(bool* bounce,const Circle* circle,const PlayerBox* box) {
	int dX = abs(circle->pos.x - box->pos.x);
	int dY = abs(circle->pos.y - box->pos.y);
	int rangeX = circle->radius + box->length.x / 2;
	int rangeY = circle->radius + box->length.y / 2;

	//円の中心点が矩形の縦と横の長さより外側の場合 かつ、円の横移動の向きが矩形の角の位置が反対である場合(例.左方向→矩形の右角)
	if ((dX <= rangeX && dX > box->length.x / 2) && (dY <= rangeY && dY > box->length.y / 2)
		&& (circle->pos.x - box->pos.x) * circle->circleScalar.x < 0) {
		return 2;
	}
	//円の中心点が矩形の縦の長さより外側、横の長さより内側の場合
	else if((dX <= rangeX && dX <= box->length.x / 2) && (dY <= rangeY && dY > box->length.y / 2)){
		return 1;
	}
	//円の中心点が矩形の中心点より下側の場合
	else if(dX <= rangeX && (dY <= rangeY && dY <= box->length.y / 2)) {
		return 0;
	}
	//それ以外(当たっていない場合)
	else {
		*bounce = false;
		return -1;
	}
}

static void Draw(const int score, const int highScore,const int img, const Circle* circle, const PlayerBox* box) {
	//背景
	DrawGraph(0, 0, img, TRUE);

	//円
	DrawCircle(circle->pos.x, circle->pos.y, circle->radius, RED, TRUE);
	DrawCircle(circle->pos.x - circle->radius / 4, circle->pos.y - circle->radius / 4, circle->radius / 2, 0xffa0a0, TRUE);
	DrawCircle(circle->pos.x - circle->radius / 4, circle->pos.y - circle->radius / 4, circle->radius / 4, WHITE, TRUE);
	
	//矩形
	DrawBox(box->pos.x - box->length.x / 2 - 2, box->pos.y - box->length.y / 2 - 2, box->pos.x + box->length.x / 2, box->pos.y + box->length.y / 2, 0x40c0ff, TRUE);
	DrawBox(box->pos.x - box->length.x / 2, box->pos.y - box->length.y / 2, box->pos.x + box->length.x / 2 + 2, box->pos.y + box->length.y / 2 + 2, 0x204080, TRUE);
	DrawBox(box->pos.x - box->length.x / 2, box->pos.y - box->length.y / 2, box->pos.x + box->length.x / 2, box->pos.y + box->length.y / 2, 0x0080ff, TRUE);

	//スコア
	SetFontSize(30);
	DrawFormatString(0, 30, WHITE, "Score:%d点", score);
	DrawFormatString(WIDTH - 250, 30, YELLOW, "HighScore:%d点", highScore);
}

static void Reset(int* score,Circle* circle,const Circle* resetCircle, PlayerBox* box,const PlayerBox* resetBox) {
	*score = 0;
	circle->pos.x = resetCircle->pos.x;
	circle->pos.y = resetCircle->pos.y;
	circle->circleScalar.x = resetCircle->circleScalar.x;
	circle->circleScalar.y = resetCircle->circleScalar.y;
	box->pos.x = resetBox->pos.x;
	box->pos.y = resetBox->pos.y;
}