#pragma once
#include "stdafx.h"
#include "Drama.h"

class Game {
public:
	static bool loadDramas();
	static void mainLoop();

private:
	static vector<drama> dramas;
};
