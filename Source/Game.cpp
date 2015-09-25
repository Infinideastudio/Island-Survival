#include "stdafx.h"
#include "Game.h"
#include "Console.h"
#include <fstream>
#include <sstream>
#include <Windows.h>
#pragma warning(disable:4706) //�������ʽ�ڵĸ�ֵ
#pragma warning(disable:4127) //�������ʽ�ǳ���
vector<drama> Game::dramas;
map<string, int> Game::vars;
unsigned int Game::dramaNow = 1;

bool Game::loadDramas()
{
	int i = 1;
	bool loadSuccess = false;
	std::stringstream ss;
	std::ifstream ifs;
	ifs.open("Dramas\\1.dat", std::ios::in | std::ios::binary); //���ļ�
	while (ifs) {
		int id;
		ifs.read((char*)&id, sizeof(int));
		if (id != i) { //У��ID�Ƿ���ȷ
			loadSuccess = false;
			break;
		}

		loadSuccess = true;
		drama dm;

		int length;

		//��ʼ��ȡ����
		ifs.read((char*)&length, sizeof(int));
		if (length == 0) {
			loadSuccess = false;
			break;
		}
		auto description = std::auto_ptr<char>(new char[length + 1]);
		ifs.read(description.get(), length);
		description.get()[length] = '\0';
		dm.description = description.get();
		//��ʼ��ȡѡ��
		ifs.read((char*)&length, sizeof(int));
		if (length == 0) {
			dm.isEnding = true;
		}
		else {
			dm.isEnding = false;
			auto options = std::auto_ptr<char>(new char[length + 1]);
			ifs.read(options.get(), length);
			options.get()[length] = '\0';
			//ѭ���и�ѡ��
			char* next_token = nullptr;
			char* option = strtok_s(options.get(), "/", &next_token);
			while (option != nullptr)
			{
				optionPackage opt;
				string optstr(option);
				//����ɾ�����з�
				for (auto iter = optstr.begin(); iter != optstr.end();) {
					if (*iter == '\r' || *iter == '\n') iter = optstr.erase(iter);
					else iter++;
				}
				//��ʽ��ѡ���������$ѡ������#ѡ��Ľ��
				int optreqpos = optstr.find("$");
				int optrespos = optstr.find("#");
				if (optrespos == -1) {  //��������ѡ���û�н���ű�
					loadSuccess = false;
					break;
				}
				opt.result = optstr.substr(optrespos + 1);
				if (optreqpos != -1) { //������ű�
					opt.requirement = optstr.substr(0, optreqpos);
					opt.text = optstr.substr(optreqpos + 1, optrespos - optreqpos - 1);
				}
				else {
					opt.text = optstr.substr(0, optrespos);
				}
				
				dm.options.push_back(opt);
				option = strtok_s(nullptr, "/", &next_token);
			}
		}

		ss.clear();
		ss.str("");
		ifs.close();

		if (!loadSuccess) break; //�ڲ���ش���

		dramas.push_back(dm);

		i++;
		ss << "Dramas\\" << i << ".dat";
		ifs.open(ss.str(), std::ios::in | std::ios::binary); //���ļ���Ϊ��һ��ѭ����׼��
	}
	return loadSuccess;
}

void Game::mainLoop()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	if (dramas.size() == 0) return;
	bool gameIsRunning = true;
	while (gameIsRunning) {
		drama& dmNow = dramas[dramaNow - 1];
		if (dmNow.isEnding)
		{
			gameOver(dmNow.description);
			gameIsRunning = false;
		}
		else {
			Console::showDescription(dmNow.description);
			Console::showOptions(&dmNow.options);
			evalResult(dmNow.options[Console::waitForChoose() - 1].result);
			system("cls");
		}
	}
}

string getNum(string str) {
	string ret;
	unsigned int pos = 0;
	while (pos<str.length()) {
		if (isdigit(str[pos])) ret += str[pos];
		else break;
		pos++;
	}
	return ret;
}

std::pair<string, int> tryToGetVarName(string sentence) {
	if (sentence.substr(0, 1) == "{") {
		int l = sentence.find("}");
		if (l != -1) {
			return std::make_pair(sentence.substr(1, l - 1), l);
		}
	}
	return std::make_pair("", 0);
}

void Game::evalResult(string result, string split)
{
	char* next_token = nullptr;
	auto str = std::auto_ptr<char>(new char[result.length() + 1]);
	strcpy_s(str.get(), result.length() + 1, result.data());
	char* sentence = strtok_s(str.get(), ";", &next_token);
	do
	{
		string strsentence = sentence;
		if (strsentence.substr(0, 4) == "goto") {
			dramaNow = atoi(strsentence.substr(5).c_str());
			if (dramaNow > dramas.size()) Console::showError("����ָ��" + strsentence + "ִ��ʧ�ܣ�");
			continue;
		}
		else {
			auto varp = tryToGetVarName(sentence);
			if (varp.second != 0) {
				string var = varp.first;
				string mode = strsentence.substr(varp.second + 1, 1);
				string strnum = getNum(strsentence.substr(varp.second + 2));
				if (mode == "+") vars[var] += atoi(strnum.c_str());
				if (mode == "-") vars[var] -= atoi(strnum.c_str());
			}
		}
	} while (sentence = strtok_s(nullptr, ";", &next_token));
}

bool Game::evalBoolean(string sentence)
{
	if (sentence == "") return true;
	bool result = true;
	bool first = true;
	int flag = -1; //0 && , 1 ||
	string str;
	unsigned int pos = 0;
	while (true) {
		if (pos >= sentence.size()) break;
		if (sentence[pos] != '&' && sentence[pos] != '|') str += sentence[pos];
		else break;
		pos++;
	}
	
	while (true) {
		auto varp = tryToGetVarName(str);
		if (varp.second == 0) return true;
		string var = varp.first;
		string mode = str.substr(varp.second + 1, 1);
		string strnum = getNum(str.substr(varp.second + 2));
		if ((mode == ">" && vars[var] > atoi(strnum.c_str())) || (mode == "<" && vars[var] < atoi(strnum.c_str()))) {
			if (first) result = true;
			else if (flag == 0) result = result && true;
			else result = true;
		}
		else {
			if (first) result = false;
			else if (flag == 0) result = false;
			else result = result || false;
		}
		first = false;
		str = "";
		flag = sentence[pos] == '&' ? 0 : 1;
		pos++;
		//��·����
		if (flag == 0 && result == false) return false;
		if (flag == 1 && result == true) return true;

		while (true) {
			if (pos >= sentence.size()) {
				if (str != "") break;
				else return result;
			}
			if (sentence[pos] != '&' && sentence[pos] != '|') str += sentence[pos];
			else break;
			pos++;
		}
		
	}
	return false;
}

void Game::gameOver(string reason)
{
	Console::showText(reason, true);
	Console::showText("��Ϸ������", true);
}
