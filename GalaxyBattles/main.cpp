// GalaxyBattles.cpp: определяет точку входа для приложения.
//
#pragma once
#include <Engine.hpp>
#include <tiny_obj_loader.cc>
#include <vector>
#include <chrono>
#include <EventsManager.hpp>
#include <Sound.hpp>
#include <iostream>
#include "Game/Tank.hpp"

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/*read .obj file with tinyobj library*/
void read_file(std::string_view file_name, tinyobj::ObjReader& reader) {
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./";

	if (!reader.ParseFromFile(file_name.data(), reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty()) {
		std::cout << "TinyObjReader: " << reader.Warning();
	}
}

std::vector<Engine::Triangle> get_triangle_list(std::string_view file_name) {
	tinyobj::ObjReader reader;
	read_file(file_name, reader);

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();

	std::vector<Engine::Triangle> res;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			Engine::Triangle tr;

			tr.v1.x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 0];
			tr.v1.y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 0].vertex_index) + 1];
			tr.v2.x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 0];
			tr.v2.y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 1].vertex_index) + 1];
			tr.v3.x = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 0];
			tr.v3.y = attrib.vertices[3 * size_t(shapes[s].mesh.indices[index_offset + 2].vertex_index) + 1];

			res.push_back(tr);
			index_offset += fv;
		}
	}
	return res;
}

int main(int argc, char* argv[])
{
	std::shared_ptr<EventsManager> em = std::make_shared<EventsManager>();
	std::shared_ptr<AudioManager> am = std::make_shared<AudioManager>();
	Engine engine{ em, am};
	
	string mode = "OpenGL";
	if (argc > 1)
	{
		mode = argv[1];
	}

	engine.init("GalaxyBattles", SCREEN_WIDTH, SCREEN_HEIGHT, mode);
	std::shared_ptr<Tank> tank = std::make_shared<Tank>(engine, am);
	em->add_delegate(tank.get());
	engine.scene()->addNode(tank);
	
	std::shared_ptr<Sound> _music_back = am->createSound("../../../../GalaxyBattles/EtanolEngine/resource/back_short_ev.wav", true, 10);
	//std::shared_ptr<Sound> _tank_shot = am->createSound("../../../../GalaxyBattles/EtanolEngine/resource/shot_ev.wav", true, 100);
	_music_back->play();
	//_tank_shot->play();
	while (engine.isActive()) {
		engine.update();
	}

	return 0;
}
