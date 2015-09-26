#pragma once
#include "stdafx.h"
#include "Drama.h"

class Game {
public:
	static void setDramasPath(string path);
	static void showWelcome();
	static bool loadDramas();
	static void mainLoop();
	static void evalResult(string result, string split = ";");
	static bool evalBoolean(string sentence);
	static void gameOver(string reason);
	static void saveGame(string id);
	static void loadGame(string id);
private:
	static vector<drama> dramas;
	static map<string, int> vars;
	static unsigned int dramaNow;
	static string dramasPath;
};
