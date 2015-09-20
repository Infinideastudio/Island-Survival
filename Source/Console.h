#pragma once
#include "stdafx.h"
#include "Drama.h"

class Console {
public:
	//��ʾ������Ϣ
	inline static void showDescription(std::string text)
	{
		showText(text, true);
	}

	//��ʾ����ѡ��
	static void Console::showOptions(const vector<optionPackage>* opts);

	//�ȴ����ѡ��
	static int waitForChoose();

	//��ʾ������Ϣ
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