#include "stdafx.h"
#include "Console.h"
#include "Game.h"

int main() {
	if(!Game::loadDramas()) Console::showError("�����޷���ȡ��Ϸ���ݣ�");
	Game::mainLoop();
	system("pause");
	return 0;
}