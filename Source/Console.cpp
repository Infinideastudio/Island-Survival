#include "stdafx.h"
#include "Console.h"
#include "Drama.h"
#include "Game.h"
#include <time.h>

const vector<optionPackage>* Window::options;
map<char, char> Window::inputToOptions;
GLFWwindow* Window::win;
int Window::screen_w, Window::screen_h;
int Window::lineHeight = 30;
Window::point Window::pos;
bool Window::gamePause = false;

void Window::createWindow(int w, int h, bool r)
{
	screen_w = w;
	screen_h = h;
	if (r) {
		w *= 2;
		h *= 2;
	}
	glfwInit();
	win = glfwCreateWindow(w, h, "Island-Survival", NULL, NULL);
	glfwSetWindowSizeCallback(win, [](GLFWwindow*, int w, int h) {
		screen_h = h;
		screen_w = w;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, w, h, 0.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	});
	glfwMakeContextCurrent(win);
	glewInit();
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, w / 2, h / 2, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	selectFont(60, ANSI_CHARSET, "微软雅黑 Light");
}

void Window::showOptions(const vector<optionPackage>* opts)
{
	char i = 0;
	char realid = 0;
	options = opts;
	inputToOptions.clear();
	
	for each (optionPackage opt in *opts)  //遍历输出所有选项
	{
		realid++;
		if (Game::evalBoolean(opt.requirement)) {
			if (opt.text == "") {
				Game::evalResult(opt.result);
			}
			else {
				i++;
				std::stringstream ss;
				ss << (int)i << ". " + opt.text;
				showText(ss.str());
				inputToOptions[i] = realid;
			}
		}
	}
}

bool Window::waitForChoose(int& choose)
{
	if (glfwGetKey(win,GLFW_KEY_S)) Game::saveGame("1");
	if (glfwGetKey(win, GLFW_KEY_L)) {
		Game::loadGame("1");
		choose = -1;
		glfwPollEvents();
		return true;
	}
	unsigned char c = 0;
	bool keyDown = false;
	for (char i = 0; i != 10; i++) {  //循环测试数字键按下情况
		if (glfwGetKey(win, GLFW_KEY_0 + i)) {
			c = i;
			keyDown = true;
			break;
		}
	}
	if (keyDown && c > 0 && c <= options->size() && inputToOptions.find(c) != inputToOptions.end()) {   //判断按键是否有效
		choose = inputToOptions[c];
		glfwPollEvents();
		return true;
	}
	else {
		return false;
	}
}

void Window::update(double waitSec)
{
	//wait 要暂停的秒数
	if (glfwWindowShouldClose(win)) exit(0); //有内存泄露可能，注意
	if (!gamePause && waitSec == 0) {
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	else if (waitSec > 0) {
		glfwSwapBuffers(win);
		double timeStart = (double)clock() / CLOCKS_PER_SEC;
		while ((double)clock() / CLOCKS_PER_SEC < timeStart + waitSec) {
			glfwPollEvents();
		}
	}
	else {
		glfwSwapBuffers(win);
		while (!glfwGetKey(win, GLFW_KEY_SPACE) &&
			!glfwGetKey(win, GLFW_KEY_ENTER) &&
			!glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT)) {
			glfwPollEvents();
		}
		gamePause = false;
	}
}

void gotoxy(short x, short y)
{
	HANDLE hOut;
	COORD pos = { x, y };
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
