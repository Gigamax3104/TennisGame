#pragma once
#include	"DxLib.h"
#include	<time.h>

#define WIDTH 960 
#define HEIGHT 640
#define WHITE 0xffffff
#define RED 0xff0000
#define YELLOW 0xffff00
#define OLANGE 0xee7800

//シーンの列挙体
enum Scene {
	TITLE,
	GAME,
	GAMEOVER
};

//位置・幅等に用いる構造体
struct Vector2 {
	int x;
	int y;
};

//円の構造体
struct Circle {
	Vector2 pos;
	int radius;
	Vector2 circleScalar;
};

//プレイヤーが動かす矩形の構造体
struct PlayerBox {
	Vector2 pos;
	Vector2 length;
	int boxScalar;
};

//ブロック崩しみたいに作成したかったけど、時間がなかったため、下の構造体は無しにした。
//struct TargetBox {
//	Vector2 pos;
//	Vector2 length;
//};

//共有する関数
void Game(int* scene, const int img, const int* bgm, bool* reset, Circle* circle,const Circle* resetCircle, PlayerBox* box,const PlayerBox* resetBox,time_t nowTime);