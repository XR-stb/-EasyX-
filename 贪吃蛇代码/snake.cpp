#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
#include<mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

const int BlOCK_SIZE = 20;//方块的边长
const int HEIGHT = 30;//一列方块的个数
const int WIDTH = 30;//一行方块的个数
int BLOCK[HEIGHT][WIDTH] = { 0 };//地图
char movedirection = 'd';//开始时候蛇头朝右边移动
char last_movedirection = 'd';
bool isFaild = false;//判断游戏是否结束

int food_x = 0, food_y = 0;
static int snake_length = 1;

void init_game() {
	srand((unsigned)time(NULL));
	initgraph(WIDTH * BlOCK_SIZE, HEIGHT * BlOCK_SIZE);
	setlinecolor(RGB(200, 200, 200)); // 设置线条颜色
	cleardevice(); // 清屏
	mciSendString(_T("open gameMusic.mp3 alias bgmusic"), NULL, 0, NULL);
	mciSendString(_T("play bgmusic repeat"), NULL, 0, NULL);
	BLOCK[HEIGHT / 2][WIDTH / 2] = 1;//蛇头为1
	for (int i = 1; i <= 4; i++) {
		BLOCK[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	}

	food_x = rand() % (WIDTH - 5) + 2;;
	food_y = rand() % (HEIGHT - 5) + 2;
}

void show() {//画面的显示
	//cleardevice();

	if (isFaild) {
		setbkmode(TRANSPARENT);//设置字体部分背景透明
		settextcolor(RGB(255, 0, 0));
		settextstyle(80, 0, _T("宋体"));
		// 输出字符串（Unicode 字符集）
		wchar_t s[] = L"Game Over!";
		outtextxy(130, 200, s);
		return;
	}

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (BLOCK[i][j] == 0) {
				setfillcolor(RGB(150, 150, 150));
			}
			else {
				setfillcolor(HSVtoRGB(BLOCK[i][j] * 10.0, 0.9, 1.0));
			}
			fillrectangle(j * BlOCK_SIZE, i * BlOCK_SIZE, (j + 1) * BlOCK_SIZE, (i + 1) * BlOCK_SIZE);//注意别写反
		}
	}
	setfillcolor(RGB(0, 255, 0));
	fillrectangle(food_x * BlOCK_SIZE, food_y * BlOCK_SIZE, (food_x + 1) * BlOCK_SIZE, (food_y + 1) * BlOCK_SIZE);

	FlushBatchDraw(); // 批量绘制
}

void collision_detection(int head_x, int head_y) {//碰撞检测
	if (head_x < 0 || head_x >= WIDTH || head_y < 0 || head_y >= HEIGHT || BLOCK[head_y][head_x] > 0) {
		isFaild = true;
	}
}

void coordinate_change(int& head_x, int& head_y) {//计算新蛇头的坐标
	if (movedirection == 'w') head_y -= 1;
	else if (movedirection == 's') head_y += 1;
	else if (movedirection == 'a') head_x -= 1;
	else if (movedirection == 'd') head_x += 1;
}

void snake_move() {
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (BLOCK[i][j] > 0) BLOCK[i][j] += 1;
		}
	}

	int head_x = 0, head_y = 0, tail_x = 0, tail_y = 0;
	int tail = 0;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (BLOCK[i][j] > tail) {//找尾巴，最大值
				tail = BLOCK[i][j];
				tail_x = j, tail_y = i;//坐标不要写反
			}
			if (BLOCK[i][j] == 2) {//找头，最小值 2 因为前面已经被更新
				head_x = j, head_y = i;
			}
		}
	}

	coordinate_change(head_x, head_y);//计算新蛇头的位置
	collision_detection(head_x, head_y);
	BLOCK[head_y][head_x] = 1;

	if (head_x == food_x && head_y == food_y) {//吃到食物时，刷新食物位置
		food_x = rand() % (WIDTH - 5) + 2;
		food_y = rand() % (HEIGHT - 5) + 2;
		while (BLOCK[food_y][food_x] != 0) {//防止食物刷新在蛇身上
			food_x = rand() % (WIDTH - 5) + 2;
			food_y = rand() % (HEIGHT - 5) + 2;
		}
	}
	else {
		BLOCK[tail_y][tail_x] = 0;
	}

	Sleep(100);//避免控制小蛇加速移动过快
}

void move_with_input() {
	if (_kbhit() && !isFaild) {
		char input = _getch();
		if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
			if (input == 'w' && last_movedirection != 's' ||
				input == 'a' && last_movedirection != 'd' ||
				input == 's' && last_movedirection != 'w' ||
				input == 'd' && last_movedirection != 'a') {
				movedirection = input;
			}
			else {
				movedirection = last_movedirection;
			}
			snake_move();
			last_movedirection = movedirection;
		}
	}
}

void move_without_input() {
	if (isFaild) return;
	static int wait = 0;
	wait++;
	if (wait == 10) {//避免小蛇无输入下连续快速移动造成的画面卡顿
		snake_move();
		wait = 0;
	}
}

int main() {
	init_game();
	while (1) {
		show();
		move_with_input();
		move_without_input();
	}

	_getch();
	closegraph();
	return 0;
}
