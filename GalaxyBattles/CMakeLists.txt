# CMakeList.txt: проект CMake для GalaxyBattles; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
# Добавьте источник в исполняемый файл этого проекта.
add_executable (GalaxyBattles "main.cpp")

set_target_properties(GalaxyBattles PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

add_subdirectory("EtanolEngine")
target_link_libraries(GalaxyBattles EtanolEngine)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
