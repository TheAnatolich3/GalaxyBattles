#include <string>
#include <iostream>
#include "tiny_obj_loader.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class Engine {
public:
	explicit Engine();
	~Engine();
	void init(std::string name_window);
	void update();
	bool isActive();

	void draw_figure(const std::string& file_name);
private:
	
	struct Engine_pimp;
	bool is_active;
	std::unique_ptr<Engine_pimp> _pmp;
};

