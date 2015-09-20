#include "stdafx.h"
#include "Console.h"

std::vector<std::string> Console::options;
void Console::showOptions()
{
	int i = 0;
	for each (std::string str in options)  //�����������ѡ��
	{
		showText(++i);
		showText(". " + str, true);
	}
	options.clear();
}

void Console::showResult(std::string text)
{
	showText(text);
	newline();
}

int Console::waitForChoose()
{
	unsigned int c;
	do {
		c = _getch() - '0';
	} while (c <= 0 || c > options.size());  //�жϰ����Ƿ���Ч
	return c;
}
