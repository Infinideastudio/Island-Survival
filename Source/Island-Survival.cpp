#include "stdafx.h"
#include "Window.h"
#include "Game.h"

int main(int argc, char *argv[]) {
	if (argc > 1) Game::setDramasPath(argv[1]);
	Window::createWindow(850, 500, false);
	Game::showWelcome();
	if(!Game::loadDramas()) Window::showError("�����޷���ȡ��Ϸ���ݣ�");
	Game::mainLoop();
	return 0;
}