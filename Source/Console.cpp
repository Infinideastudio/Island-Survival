#include "stdafx.h"
#include "Console.h"
#include "Drama.h"
#include "Game.h"
#include <Windows.h>

const vector<optionPackage>* Console::options;
map<char, char> Console::inputToOptions;

void Console::showOptions(const vector<optionPackage>* opts)
{
	char i = 0;
	char realid = 0;
	options = opts;
	inputToOptions.clear();
	
	for each (optionPackage opt in *opts)  //�����������ѡ��
	{
		realid++;
		if (Game::evalBoolean(opt.requirement)) {
			if (opt.text == "") {
				Game::evalResult(opt.result);
			}
			else {
				showText(static_cast<int>(++i));
				showText(". " + opt.text, true);
				inputToOptions[i] = realid;
			}
		}
	}
}

int Console::waitForChoose()
{
	unsigned char c;
	do {
		c = (char)_getch();
		//if (options->size() == 1) return 1; //ֻ��һ��ѡ��
		if (c == 's') Game::saveGame("1");
		if (c == 'l') {
			Game::loadGame("1");
			return -1;
		}
		c -= '0';
	} while (c <= 0 || c > options->size() || inputToOptions.find(c) == inputToOptions.end());  //�жϰ����Ƿ���Ч
	return inputToOptions[c];
}

void gotoxy(short x, short y)
{
	HANDLE hOut;
	COORD pos = { x, y };
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
