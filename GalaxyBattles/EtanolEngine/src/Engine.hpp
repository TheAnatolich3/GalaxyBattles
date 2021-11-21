#include <cstdio>
#include <string>
#include <memory>
#include <iostream>
#include "tiny_obj_loader.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

class Engine {
public:
	explicit Engine();
	~Engine();
	void init(std::string name_window);
	void update();
	bool isActive();
	void draw_figure(const std::string& file_name);
private:
	void read_file(const std::string& file_name, tinyobj::ObjReader& reader);
	void draw_line(int x0, int y0, int x1, int y1);

	struct Pimp;
	bool is_active;
	std::unique_ptr<Pimp> _pmp;
};