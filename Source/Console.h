#pragma once
#include "stdafx.h"

class Console {
public:
	//��ʾ������Ϣ
	inline static void showDescription(std::string text)
	{
		showText(text, true);
	}

	//���һ��ѡ��
	inline static void pushOption(std::string text)
	{
		options.push_back(text);
	}

	//��ʾ����ѡ��
	static void showOptions();

	//��ʾ���غϽ�������ѡ��
	static void showResult(std::string text);

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
	static std::vector<std::string> options;
};