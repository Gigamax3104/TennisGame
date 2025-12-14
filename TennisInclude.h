#pragma once
#include	"DxLib.h"
#include	<time.h>

#define WIDTH 1280 
#define HEIGHT 720
#define WHITE 0xffffff
#define YELLOW 0xffff00
#define OLANGE 0xee7800

enum Scene {
	TITLE,
	GAME,
	GAMEOVER
};

struct Vector2 {
	int x;
	int y;
};

struct Circle {
	Vector2 pos;
	int radius;
	Vector2 circleScalar;
};

struct Box {
	Vector2 pos;
	Vector2 length;
	int boxScalar;
};

void Game(int* scene, Circle* circle, Box* box,time_t nowTime);