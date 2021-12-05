// GalaxyBattles.cpp: определяет точку входа для приложения.
//
#pragma once
#include <Engine.hpp>
#include <Sprite.hpp>
#include <tiny_obj_loader.cc>
#include <vector>
#include <chrono>

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
 	Engine engine{};
	string mode = "OpenGL";
	if (argc > 1)
	{
		mode = argv[1];
	}

	engine.init("GalaxyBattles", SCREEN_WIDTH, SCREEN_HEIGHT, mode);

	auto tank_body = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_body_removed_back.png");
	auto tank_head = std::make_shared<Sprite>(engine, "../../../../GalaxyBattles/EtanolEngine/resource/tank_head_removed_back.png");
	tank_body->setPosition(glm::vec2(engine.get_window_width() * 0.2f,
									 engine.get_window_height()* 0.5f));
	tank_head->setScale(glm::vec2(0.6f, 0.6f));

	glm::vec2 tank_body_size = tank_body->getSize();
	glm::vec2 tank_head_size = tank_head->getSize();
	glm::vec2 tank_head_scale = tank_head->getScale();

	tank_head->setPosition(glm::vec2(tank_body_size.x * 0.6 - (tank_head_size.x * 0.5 * tank_head_scale.x),
		tank_body_size.y / 2 - (tank_head_size.y * 0.5 * tank_head_scale.y)));

	tank_head->setAnchor(glm::vec2(tank_head_size.x*0.23, tank_head_size.y*0.29));
	

	engine.scene()->addNode(tank_body);
	tank_body->addNode(tank_head);
	//engine.load_picture(get_triangle_list("../../../../GalaxyBattles/EtanolEngine/resource/african_head.obj"));
	while (engine.isActive()) {
		engine.update();
	}
	return 0;
}
