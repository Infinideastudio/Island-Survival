#include "stdafx.h"
#include "Console.h"
#include "Game.h"

int main(int argc, char *argv[]) {
	if (argc > 1) Game::setDramasPath(argv[1]);
	Window::createWindow(850, 500, true);
	Game::showWelcome();
	if(!Game::loadDramas()) Window::showError("�����޷���ȡ��Ϸ���ݣ�");
	Game::mainLoop();
	return 0;
}