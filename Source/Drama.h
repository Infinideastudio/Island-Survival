#pragma once
#include "stdafx.h"

struct optionPackage {
	string text;
	string requirement;
};

struct drama {
	int id;
	bool isEnding;
	string description;
	vector<optionPackage> options;
};