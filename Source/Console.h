#pragma once
#include "stdafx.h"
#include "Drama.h"
void gotoxy(short x, short y);
class Window {
public:
	static int screen_w, screen_h;

	//��ʼ������
	static void createWindow(int w, int h, bool r = false);

	//��ô���
	inline static GLFWwindow* getWindow() { return win; }

	//��ʾ������Ϣ
	inline static void showDescription(std::string text)
	{
		showText(text);
	}

	//��ʾ����ѡ��
	static void showOptions(const vector<optionPackage>* opts);

	//������ѡ��
	static bool waitForChoose(int& choose);

	//��ʾ������Ϣ
	inline static void showError(std::string text)
	{
		glfwDestroyWindow(win);
		glfwTerminate();
		std::cout << text << std::endl;
		system("pause");
		exit(-1);
	}

	template <typename T>
	inline static void showText(T text) {
		//std::cout << text;
		std::stringstream ss;
		ss << text;
		glRasterPos2i(pos.x, pos.y);
		drawString(ss.str());
		newline();
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
	static void selectFont(int size, int charset, const char* face) {
		HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
			charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
		HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
		DeleteObject(hOldFont);
	}

	static void drawString(string str) {
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

		// ������ַ�ת��Ϊ���ַ�
		wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.c_str(), -1, wstring, len);
		wstring[len] = L'\0';

		// �������ַ�
		for (int i = 0; i < len; ++i)
		{
			wglUseFontBitmapsW(hDC, wstring[i], 1, list);
			glCallList(list);
		}

		// ����������ʱ��Դ
		free(wstring);
		glDeleteLists(list, 1);
	}
	
	inline static void newline(int = 0, int offsetY = lineHeight) {
		//pos.x = offsetX;
		pos.y += offsetY;
	}

	static int lineHeight;
	static const vector<optionPackage>* options;
	static GLFWwindow* win;
	struct point {
		int x, y;
	};
	static point pos;
	static map<char, char> inputToOptions;
	static bool gamePause;
};