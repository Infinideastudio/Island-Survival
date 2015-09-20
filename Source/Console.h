#pragma once
#include "stdafx.h"

class Console {
public:
	//显示描述信息
	inline static void showDescription(std::string text)
	{
		showText(text, true);
	}

	//添加一个选项
	inline static void pushOption(std::string text)
	{
		options.push_back(text);
	}

	//显示所有选项
	static void showOptions();

	//显示本回合结果并清空选项
	static void showResult(std::string text);

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
	static std::vector<std::string> options;
};