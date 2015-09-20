#pragma once
#include "stdafx.h"
#include "Drama.h"

class Game {
public:
	static bool loadDramas();
	static void mainLoop();
	static void evalResult(string result);
	static void gameOver(string reason);

private:
	static vector<drama> dramas;
	static unsigned int dramaNow;

};
