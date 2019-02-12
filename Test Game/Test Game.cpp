// Test Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include "../overpeek-engine/utility/filereader.h"

int main()
{
	float speed = std::stof(oe::readSetting("settings.ini", "PlayerSpeed"));

	speed *= 2.0;

	oe::writeSetting("settings.ini", "PlayerSpeed", std::to_string(speed));




	speed = std::stof(oe::readSetting("settings.ini", "TimeSpeed"));

	speed *= 0.5;

	oe::writeSetting("settings.ini", "TimeSpeed", std::to_string(speed));
}