#include <fstream>
#include <string>
#include "GLDrawProgram.hpp"

std::string read_file(const std::string_view file_name)
{
	std::string line, text;
	std::ifstream in(file_name.data());

	if (in.is_open())
	{
		while (!in.eof())
		{
			getline(in, line);
			text += line;
			text += '\n';
		}
	}
	in.close();
	return text;
}

GLDrawProgram::GLDrawProgram()
    : GLProgram(read_file("../../../../GalaxyBattles/EtanolEngine/src/shader.vert"),
				read_file("../../../../GalaxyBattles/EtanolEngine/src/frag_shader.vert"))
{

}