#pragma once
#include "stdafx.h"
#include "Drama.h"

class Console {
public:
	//显示描述信息
	inline static void showDescription(std::string text)
	{
		showText(text, true);
	}

	//显示所有选项
	static void Console::showOptions(const vector<optionPackage>* opts);

	//等待玩家选择
	static int waitForChoose();

	//显示错误信息
	inline static void showError(std::string text)
	{
		showText(text, true);
		system("pause");
		exit(-1);
	}

	template <typename T>
	inline static void showText(T text, bool newl = false) {
		std::cout << text;
		if (newl) newline();
	}

private:
	inline static void newline() {
		std::cout << std::endl;
	}
	const static vector<optionPackage>* options;
};