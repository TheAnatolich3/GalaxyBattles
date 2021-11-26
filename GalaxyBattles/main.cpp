// GalaxyBattles.cpp: определяет точку входа для приложения.
//
#pragma once

#include "EtanolEngine/src/Engine.hpp"
using namespace std;

int main()
{
	Engine engine{};
	engine.init("GalaxyBattles");
	//engine.draw_figure("../../../../GalaxyBattles/EtanolEngine/resource/african_head.obj");

	while (engine.isActive()) {
		engine.update();
	}
	return 0;
}
