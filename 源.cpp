#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <mmstream.h>

#pragma comment(lib, "winmm.lib")
#define NUM 200
#define SIZE 10

enum Ch { up = 72, down = 80, left = 75, right = 77 };

// ����ṹ
struct Coor {
	int x;
	int y;
};

struct Snake {
	int n;		// �ߵĽ���
	Ch ch;		// �ߵķ���
	Coor szh[NUM];
};

struct Food {
	int x;
	int y;
	bool isEat;	// �Ƿ񱻳Ե�
};

int speed = 100;
Snake snake;
Food food;

void GameInit() {
	srand((unsigned)time(NULL));	// �����������
	snake.n = 3;
	snake.ch = right;
	snake.szh[0].x = 100;
	snake.szh[0].y = 100;
	snake.szh[1].x = 90;
	snake.szh[1].y = 100;
	snake.szh[2].x = 80;
	snake.szh[2].y = 100;
	food.isEat = false;
}

void DrawGame() {
	cleardevice();
	// �Ȼ���ͷ
	setfillcolor(CYAN);
	fillrectangle(snake.szh[0].x, snake.szh[0].y, snake.szh[0].x + SIZE, snake.szh[0].y + SIZE);
	// Ȼ��ӵ�2�ڿ�ʼ��
	for (int i = 1; i < snake.n; i++) {
		rectangle(snake.szh[i].x, snake.szh[i].y, snake.szh[i].x + SIZE, snake.szh[i].y + SIZE);
	}
	// ����ʳ��
	setfillcolor(GREEN);
	fillroundrect(food.x, food.y, food.x + SIZE, food.y + SIZE, 5, 5);
}

void SnakeMove() {
	for (int i = snake.n - 1; i > 0; i--) {
		snake.szh[i].x = snake.szh[i - 1].x;
		snake.szh[i].y = snake.szh[i - 1].y;
	}
	switch (snake.ch) {
	case up:
		snake.szh[0].y -= SIZE;
		break;
	case down:
		snake.szh[0].y += SIZE;
		break;
	case left:
		snake.szh[0].x -= SIZE;
		break;
	case right:
		snake.szh[0].x += SIZE;
		break;
	}
}

void ChangeCh() {
	char key = _getch();
	switch (key) {
	case up:
		if (snake.ch != down) snake.ch = up;
		break;
	case down:
		if (snake.ch != up) snake.ch = down;
		break;
	case left:
		if (snake.ch != right) snake.ch = left;
		break;
	case right:
		if (snake.ch != left) snake.ch = right;
		break;
	}
}

void CreateFood() {
	bool flag = false;
	if (food.isEat) {
		while (!flag) {
			flag = false;
			food.x = rand() % 64 * 10;
			food.y = rand() % 48 * 10;
			for (int i = 0; i < snake.n; i++) {
				if (food.x == snake.szh[i].x && food.y == snake.szh[i].y) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				food.isEat = false;
				break;
			}
		}
	}
	//bool repeat = false;	// û��ˢ��������
	//while (1) {
	//	if (food.isEat) {
	//		food.x = rand() % 64 * 10;
	//		food.y = rand() % 48 * 10;
	//		for (int i = 0; i < snake.n; i++) {
	//			if (food.x == snake.szh[i].x && food.y == snake.szh[i].y) {
	//				repeat = true;
	//				break;
	//			}
	//		}
	//		if (repeat) continue;
	//		else {
	//			food.isEat = false;
	//			break;
	//		}
	//	}
	//	else break;
	//}
}

void EatFood() {
	if (snake.szh[0].x == food.x && snake.szh[0].y == food.y) {
		food.isEat = true;
		snake.n++;
	}
}

int GameOver() {
	if (snake.szh[0].x < 0 || snake.szh[0].x > 640 || snake.szh[0].y < 0 || snake.szh[0].y > 480) {
		return 1;
	}
	for (int i = 1; i < snake.n - 1; i++) {
		if (snake.szh[0].x == snake.szh[i].x && snake.szh[0].y == snake.szh[i].y) {
			return 1;
		}
	}
	return 0;
}

int main() {

	initgraph(640, 480);
	mciSendString(L"open bgm.wav", 0, 0, 0);
	mciSendString(L"play bgm.wav", 0, 0, 0);

	GameInit();
	int status = 0;
	while (1) {
		while (!_kbhit()) {	// kbhit() �������Ƿ񰴼�
			SnakeMove();
			DrawGame();
			CreateFood();
			EatFood();
			status = GameOver();
			if (status) break;
			Sleep(speed);
		}
		if (status) break;
		ChangeCh();
	}

	closegraph();
	//getchar();
	return 0;
}
