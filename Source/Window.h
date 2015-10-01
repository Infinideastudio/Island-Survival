#pragma once
#include "stdafx.h"
#include "Drama.h"
void gotoxy(short x, short y);
class Window {
public:
	static int screen_w, screen_h;

	//初始化窗口
	static void createWindow(int w, int h, bool r = false);

	//获得窗口
	inline static GLFWwindow* getWindow() { return win; }

	//显示描述信息
	static void showDescription(std::string text, int prog, bool& ok);

	//显示所有选项
	static void showOptions(const vector<optionPackage>* opts, int prog);

	//检测玩家选择
	static bool waitForChoose(int& choose);

	//显示错误信息
	inline static void showError(std::string text)
	{
		glfwDestroyWindow(win);
		glfwTerminate();
		std::cout << text << std::endl;
		system("pause");
		exit(-1);
	}

	template <typename T>
	inline static void showText(T text, bool line = true) {
		//std::cout << text;
		std::stringstream ss;
		ss << text;
		glRasterPos2i(pos.x, pos.y);
		drawString(ss.str());
		if(line) newline();
	}

	inline static void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	inline static void setBackgroundColor(float r, float g, float b) { glClearColor(r, g, b, 1); }

	inline static void setColor(float r, float g, float b) { glColor3f(r, g, b); }
	 
	inline static void initCurPos(int x = Window::screen_w / 10, int y = Window::screen_h / 10) { pos.x = x; pos.y = y; }

	inline static void setCurPosCenter(int offsety = 0) { initCurPos(screen_w / 2, screen_h / 2 + offsety); }

	inline static void pause(bool showTip = false) {
		gamePause = true;
		if (showTip) {
			glRasterPos2i(20, screen_h - lineHeight);
			drawString("Space to continue.");
		}
	}

	static void update(double waitSec = 0);

private:
	static void selectFont(int size, int charset, const char* face);

	static void drawString(string str);
	
	inline static void newline(int = 0, int offsetY = lineHeight) {
		//pos.x = offsetX;
		pos.y += offsetY;
	}
	struct point {
		int x, y;
	};
	static point pos;
	static int lineHeight;
	static const vector<optionPackage>* options;
	static GLFWwindow* win;
	static map<char, char> inputToOptions;
	static bool gamePause;
};