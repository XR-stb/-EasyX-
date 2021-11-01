
#                                           Easyx 学习记录

### 1. 创建小球

![image-20211023092834830](https://i.loli.net/2021/10/23/8nHWEmclvKD6CFr.png)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
int main()
{
    initgraph(600, 600);
    fillcircle(400, 120,50);//北
    fillcircle(320, 200, 50);//西
    fillcircle(400,200, 25);//中
    fillcircle(480, 200, 50);//东
    fillcircle(400, 280, 50);//南
    _getch();//和getchar一样
    closegraph();
    return 0;
}

```

**采用常量可以使我们的代码的健壮性增强，易于维护和修改**

![image-20211023093343362](https://i.loli.net/2021/10/23/uARcemjOYXTnNKD.png)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

const int x = 300, y = 200, mid_radius = 25,others_radius = 50;

int main()
{
    initgraph(600, 600);
    fillcircle(x, y-80,others_radius);//北
    fillcircle(x-80, y, others_radius);//西
    fillcircle(x,y, mid_radius);//中
    fillcircle(x+80, y, others_radius);//东
    fillcircle(x, y+80, others_radius);//南
    _getch();
    closegraph();
    return 0;
}
```

### 2. 实现小球匀速下滑

[EasyX 白色小球 匀速下滑动画_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1XP4y1t7Rg/)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
int main()
{
    initgraph(600, 600);
    int cnt = 0;
    int x = 300, y = 100, r = 50;
    while (++cnt < 500) {
        fillcircle(x, y, r);
        y += 10;
        Sleep(200);
        cleardevice();
    }
    _getch();
    closegraph();
    return 0;
}
```

### 3. 小球往返运动

[EasyX 小球匀速往返运动_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1jh411n7o6/)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int main()
{
    initgraph(600, 600);
    int cnt = 0;
    int x = 300, y = 100, r = 50;
    int step = 10;
    while (true) {
        fillcircle(x, y, r);
        if (y >= 550) step = -10;
        if (y <= 50) step = 10;
        y += step;//实现加速运动的话，我们只需要每次加上一个变量即可
        Sleep(30);
        cleardevice();
    }
    _getch();
    closegraph();
    return 0;
}
```

### 4. 小球抛物线运动

[小球抛物线运动_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1qT4y1R7M4/)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <cmath>

int main()
{
    double width = 900, height = 600;
    initgraph(width, height);
    double x = 300, y = 100, r = 10;
    double step = 5;
    /*（1）水平方向：s = v₀×t
      （2）竖直方向：h = (1 / 2)gt²
    */
    double init_v = 10, t = 0, g = 5;
    while (true) {
        fillcircle(x, y, r);
        double s = init_v * t;
        double h = 0.5 * g *pow(t,2);
        x += s;
        y += h;
        t += 0.5;
        if (x >= width - 20 || y >= height - 20) break;
        Sleep(100);
        cleardevice();
    }
    _getch();
    closegraph();
    return 0;
}
```

### 5. 按空格小球弹起一次

[按空格小球弹起_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1iP4y1b7yg/)

```c++
#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

using std::cout;
using std::cin;
using std::endl;

int main()
{
    int width = 900, height = 600;
    initgraph(width, height);
    int cir_x = 300, cir_y = 570, cir_r = 30;
    fillcircle(cir_x, cir_y, cir_r);
    
    int rec_left = 400, rec_top = 500, rec_right = 430, rec_bottom = height;
    fillrectangle(rec_left, rec_top, rec_right, rec_bottom);

    while (1) {
        if (_kbhit()) {//检测键盘是否有输入
            char input = _getch();
            if (input == ' ') {
                auto ball_jump = [=](int x, int y) mutable {
                    int step = 0;
                    int ty = y;//记录一下原来的位置，我们只跳起一次即可
                    while (1) {
                        if (y >= 570) step = -5;
                        if (y <= 350) step = 5;
                        y += step;
                        fillcircle(x, y, cir_r);
                        fillrectangle(rec_left, rec_top, rec_right, rec_bottom);
                        if (y == ty) break;
                        
                        Sleep(20);
                        cleardevice();
                    }
                };
                ball_jump(cir_x, cir_y);
            }
        }
    }
    _getch();
    closegraph();
    return 0;
}
```

### 6. [小球躲避方块游戏演示](https://www.bilibili.com/video/BV133411k73o/)

[小球躲避方块游戏演示_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV133411k73o/)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
using namespace std;

int main() {
	double width, height, gravity;
	double ball_x, ball_y, ball_vy, radius;
	double rect_left_x, rect_top_y, rect_width, rect_height, rect_vx;
	width = 600, height = 400, gravity = 0.6;
	initgraph(width, height);

	int score = 0;
	int isBallOnFloor = 1;
	
	radius = 20;
	ball_x = width / 4, ball_y = height - radius;
	ball_vy = 0;
	
	rect_height = 100, rect_width = 20;
	rect_left_x = width * 3 / 4, rect_top_y = height - radius;
	rect_vx = -3;

	while (1) {
		if (_kbhit()) {
			char input = _getch();
			if (input == ' '&& isBallOnFloor == 1) {//如果按下空格，且小球在地面就弹起，赋予向上初速度
				ball_vy = -17;
				isBallOnFloor = 0;
			}
		}

		ball_vy += gravity;
		ball_y += ball_vy; // 小球匀速上升运动
		if (ball_y >= height - radius ) {//如果出界
			ball_vy = 0;
			ball_y = height - radius;//调整位置，避免超出地图
			isBallOnFloor = 1;
		}

		rect_left_x += rect_vx;
		if (rect_left_x <= 0) {//如果方块到达了最左边
			rect_left_x = width;//出现在最右边
			score += 1;
			rect_height = rand() % int(height / 4) + height / 4;//随机方块高度，且最低height / 4
			rect_vx = rand() / float(RAND_MAX) * 4 - 7;//随机速度 且最低速度为 -3
			//rand () 产生 0 - RAND_MAX范围的数
			//short范围 -32768~32767 RAND_MAX = 0x7fff = 32767
		}

		//碰撞检测
		if ((rect_left_x <= ball_x + radius) 
			&& (rect_left_x + rect_width >= ball_x - radius)
			&& (height - rect_height <= ball_y + radius)
			) {
			Sleep(100);
			score = 0;
		}
		cleardevice();
		fillcircle(ball_x, ball_y, radius);
		fillrectangle(rect_left_x, height - rect_height, rect_left_x + rect_width, height);
		TCHAR s[20];
		_stprintf_s(s, _T("%d"), score);
		settextstyle(40,0,_T("宋体"));
		outtextxy(50, 30, s);
		Sleep(10);
	}

	closegraph();
	return 0;
}
```

### 7. 扇形绘制与颜色设置

![image-20211028220407269](https://i.loli.net/2021/11/01/D2yjbNlOrHeQ4a9.png)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
using namespace std;

int main() {
	double width, height;
	width = 900, height = 600;
	initgraph(width, height);

	double radius = 100, x = 300, y = 300;// 画圆形的框
	int left = x - radius, top = y-radius, right = x + radius, bottom = y + radius;
	double PI = 3.14159265358;
	double stangle = 0, endangle = PI / 4;//1rad = 180 / π = 57.30°  
	// 起始弧度  和 终止弧度

	setbkcolor(WHITE);//先设置颜色、再绘图，才能得到对应颜色的绘制效果。
    //setbkcolor(RGB(255,255,255)); // 设置背景颜色为白色 RGB模式
	setlinecolor(RED);
	setfillcolor(GREEN);
	cleardevice();//清空背景 才能给背景上色
    
	circle(y, y, radius);
	solidpie(left, top, right, bottom, stangle,endangle);
	
	_getch();
	closegraph();
	return 0;
}
```

**可以绘制一个彩色拼盘**

![image-20211028225034718](https://i.loli.net/2021/11/01/qMmw7CRy9cst5V8.png)

8. 旋转圆盘——动态视觉图

![image-20211029211211736](https://i.loli.net/2021/11/01/5sEYRMVNd9mwqXj.png)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>

int main() {
	double width, height;
	width = 900, height = 600;
	initgraph(width, height);

	double radius = 100, x = 300, y = 300;// 画圆形的框
	int left = x - radius, top = y-radius, right = x + radius, bottom = y + radius;
	double PI = 3.14159265358;

	double stangle = PI / 30, endangle = PI / 20;//黑白的弧度 PI / 60
	double stangle_2 = 0, endangle_2 = PI / 30;//红青的弧度
	
	setbkcolor(WHITE);
	cleardevice();//清空才能上色
	// 起始弧度  和 终止弧度
	//画20组即可达成效果
	for(int i = 0; i < 20; i++){
		setfillcolor(GREEN);
		fillpie(left, top, right, bottom, stangle_2, endangle_2);
		stangle_2 += PI / 20;
		endangle_2 += PI / 20;

		setfillcolor(WHITE);
		fillpie(left, top, right, bottom, stangle, endangle);
		stangle += PI / 20;
		endangle += PI / 20;

		setfillcolor(RED);
		fillpie(left, top, right, bottom, stangle_2, endangle_2);
		stangle_2 += PI / 20;
		endangle_2 += PI / 20;

		setfillcolor(BLACK);
		fillpie(left, top, right, bottom, stangle, endangle);
		stangle += PI / 20;
		endangle += PI / 20;
	}

	_getch();
	closegraph();
	return 0;
}
```

**还有同心圆**

![image-20211029211714639](https://i.loli.net/2021/11/01/7UrZvjIM5e2AN4O.png)

**以及这个**

![image-20211029213132861](C:\Users\86187\AppData\Roaming\Typora\typora-user-images\image-20211029213132861.png)

### 8. HSV颜色模型

![image-20211029215204171](https://i.loli.net/2021/11/01/nVhqelscu2PKNrv.png)

> 第二个参数是模糊程度，第三个参数是灰暗程度

![image-20211029214557350](https://i.loli.net/2021/11/01/9lWo3v5L1HABaRD.png)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
#include <iostream>
#include <typeinfo>
using namespace std;

int main() {
	double width, height;
	width = 800, height = 500;
	initgraph(width, height);

	setbkcolor(WHITE);
	cleardevice();

	double PI = 3.141592653589793;
	int x = 300, y = 300, r = 100;//圆弧的圆心
	int left = x - r, top = y - r, right = x + r, bottom = y + r;//外切矩形的左上角和右下角坐标
	int step = 10;
	COLORREF color;
	for (int i = 0; i < 360; i += step) {
		color = HSVtoRGB(i, 1, 1);
		setfillcolor(color);
		fillpie(left, top, right, bottom,
                i * PI / 180, (i + step) * PI / 180);
	}

	_getch();
	closegraph();
	return 0;
}
```

### 9. line 线条的绘制

* **直线逼近曲线，xy对应坐标等距连接即可**

![image-20211101190058437](https://i.loli.net/2021/11/01/YzKHTsM9IRBeZ14.png)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>

int main() {
	double width, height;
	width = 800, height = 500;
	initgraph(width, height);

	setbkcolor(YELLOW);
	cleardevice();
	int len = 300;
	int col_x = 100, col_y = 10, col_endx = len + col_x, col_endy = col_y;//横线
	int row_x = col_x, row_y = col_y, row_endx = row_x, row_endy = len + row_y;//竖线

	setlinecolor(BLACK);

	int line_cnt = 10;
	for (int i = 1; i < line_cnt; i++) {
		line(col_x, col_y, col_endx, col_endy);
		col_x += 30;
		col_endy += 30;
	}
	_getch();
	closegraph();
	return 0;
}
```

* **然后是$11*11$棋盘的绘制**

![image-20211101190915063](https://i.loli.net/2021/11/01/53m9ZxQN21olTfv.png)

```c++
#include <cmath>
#include <time.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>

int main() {
	double width, height;
	width = 800, height = 500;
	initgraph(width, height);

	setbkcolor(YELLOW);
	cleardevice();
	int len = 300;
	int col_x = 100, col_y = 10, col_endx = len + col_x, col_endy = col_y;//竖线
	int row_x = col_x, row_y = col_y, row_endx = row_x, row_endy = len + row_y;//横线

	setlinecolor(BLACK);
	
	int line_cnt = 10;
	for (int i = 0; i <= line_cnt; i++) {
		line(col_x, col_y, col_endx, col_endy);
		line(row_x, row_y, row_endx, row_endy);
		col_y += 30;
		col_endy += 30;
		row_x += 30;
		row_endx += 30;
	}

	_getch();
	closegraph();
	return 0;
}
```

