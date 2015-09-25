#pragma once
#include "stdafx.h"
#include "Drama.h"

class Game {
public:
	static bool loadDramas();
	static void mainLoop();
	static void evalResult(string result, string split = ";");
	static bool evalBoolean(string sentence);
	static void gameOver(string reason);
	
private:
	static vector<drama> dramas;
	static map<string, int> vars;
	static unsigned int dramaNow;

};
