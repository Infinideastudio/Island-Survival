#include "stdafx.h"
#include "Game.h"
#include "Console.h"
#include <fstream>
#include <sstream>

vector<drama> Game::dramas;
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
			char* option = strtok_s(options.get(), "|", &next_token);
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
				option = strtok_s(nullptr, "|", &next_token);
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
		}
		evalResult(dmNow.options[Console::waitForChoose() - 1].result);
	}
}

void Game::evalResult(string result)
{
	char* next_token = nullptr;
	auto str = std::auto_ptr<char>(new char[result.length() + 1]);
	strcpy_s(str.get(), result.length() + 1, result.data());
	char* sentence = strtok_s(str.get(), ";", &next_token);
	while (sentence != nullptr)
	{
		string strsentence = sentence;
		if (strsentence.substr(0, 4) == "goto") {
			dramaNow = atoi(strsentence.substr(5).c_str());
			if (dramaNow > dramas.size()) Console::showError("����ָ��" + strsentence + "ִ��ʧ�ܣ�");
		}
		sentence = strtok_s(nullptr, ";", &next_token);
	}
}

void Game::gameOver(string reason)
{
	Console::showText(reason, true);
	Console::showText("��Ϸ������", true);
}
