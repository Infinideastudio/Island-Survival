#include "stdafx.h"
#include "Console.h"
#include "Game.h"

int main() {
	if(!Game::loadDramas()) Console::showError("错误：无法读取游戏内容！");
	Game::mainLoop();

	return 0;
}