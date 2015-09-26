#include "stdafx.h"
#include "Console.h"
#include "Game.h"

int main(int argc, char *argv[]) {
	if (argc > 1) Game::setDramasPath(argv[1]);
	Game::showWelcome();
	if(!Game::loadDramas()) Console::showError("错误：无法读取游戏内容！");
	Game::mainLoop();
	system("pause");
	return 0;
}