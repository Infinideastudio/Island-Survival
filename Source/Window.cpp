#include "stdafx.h"
#include "Window.h"
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
	//lineHeight = w / 15;
	if (r) {
		w *= 2;
		h *= 2;
	}
	glfwInit();
	win = glfwCreateWindow(w, h, "Island-Survival", NULL, NULL);
	glfwSetWindowSizeCallback(win, [](GLFWwindow*, int w, int h) {
		screen_h = h;
		screen_w = w;
		//lineHeight = w / 15;
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
	glOrtho(0.0, w , h , 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	selectFont(30, ANSI_CHARSET, "微软雅黑");
}

void Window::showDescription(std::string text, int prog, bool& ok)
{
	auto str = new char[text.length() + 1];
	strcpy_s(str, text.length() + 1, text.data());
	char* next_token = nullptr;
	char* txt = strtok_s(str, "\r\n", &next_token);
	int i = 0;
	bool allshow = true;
	while (txt != nullptr)
	{
		if (i >= prog) {
			allshow = false;
			break;
		}
		pos.x = Window::screen_w / 10;
		pos.y += lineHeight;
		for (unsigned int j = 0; j < strnlen_s(txt, 1024); j++) {
			if (i++ >= prog) {
				allshow = false;
				break;
			}
			bool doublechar = false;
			if (IsDBCSLeadByte(txt[j])) { //双字节
				doublechar = true;
			}
			if (doublechar)
				showText(string() + txt[j] + txt[j+1], false);
			else
				showText(txt[j], false);
			if (doublechar) j++;
			pos.x += doublechar ? 22 : 12;
		}
		txt = strtok_s(nullptr, "\r\n", &next_token);
	}
	ok = allshow;
	delete[] str;
}

void Window::showOptions(const vector<optionPackage>* opts, int prog)
{
	char i = 0;
	char realid = 0;
	options = opts;
	inputToOptions.clear();
	pos.x = Window::screen_w / 10;
	pos.y += lineHeight * 2;
	int p = 0;
	for each (optionPackage opt in *opts)  //遍历输出所有选项
	{
		if (p++ >= prog) break;
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
	if (glfwWindowShouldClose(win) || glfwGetKey(win, GLFW_KEY_ESCAPE)) { //有内存泄露可能，注意
		glfwDestroyWindow(win);
		glfwTerminate();
		exit(0);
	}
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
			if (glfwWindowShouldClose(win) || glfwGetKey(win, GLFW_KEY_ESCAPE)) { //内存泄露风险！
				glfwDestroyWindow(win);
				glfwTerminate();
				exit(0);
			}
			glfwPollEvents();
		}
		gamePause = false;
	}
}

void Window::selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void Window::drawString(string str) {
	int len = 0;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();

	for (int i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	GLuint list = glGenLists(1);

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (int i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}