#include "stdafx.h"
#include "Console.h"

int main() {
	Console::showDescription("你好！");
	Console::pushOption("选项A");
	Console::pushOption("选项B");
	Console::showOptions();
	switch (Console::waitForChoose()) {
		case 1: Console::showResult("你选择了选项1"); break;
		case 2: Console::showResult("你选择了选项2"); break;
	}

	return 0;
}