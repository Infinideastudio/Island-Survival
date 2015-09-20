#include "stdafx.h"
#include "Console.h"
#include "Drama.h"

const vector<optionPackage>* Console::options;
void Console::showOptions(const vector<optionPackage>* opts)
{
	int i = 0;
	options = opts;
	for each (optionPackage opt in *opts)  //遍历输出所有选项
	{
		showText(++i);
		showText(". " + opt.text, true);
	}
}

int Console::waitForChoose()
{
	unsigned int c;
	do {
		c = _getch() - '0';
	} while (c <= 0 || c > options->size());  //判断按键是否有效
	return c;
}
