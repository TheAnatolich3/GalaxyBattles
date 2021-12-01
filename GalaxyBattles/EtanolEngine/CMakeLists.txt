add_library(EtanolEngine STATIC src/Engine.hpp src/Engine.cpp "src/Window.hpp" "src/SDL/SDLWindow.hpp" "src/SDL/SDLWindow.cpp" "src/EventsManager.hpp" "src/EventsManager.cpp" "src/Renderer.hpp" "src/SDL/SDLRenderer.hpp" "src/SDL/SDLRenderer.cpp" "src/Renderer.cpp" "src/GL/GLRenderer.hpp" "src/GL/GLRenderer.cpp" "src/GL/Sprite.hpp" "src/GL/Sprite.cpp")

set_target_properties(EtanolEngine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

#SDL
if(NOT IOS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
	set(SDL2MAIN_LIBRARY SDL2main)
	set(SDL2_LIBRARY SDL2-static)
endif()

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glew)
	set(GLEW_LIBRARY libglew_static)


target_link_libraries(EtanolEngine PUBLIC
	${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${GLEW_LIBRARY})

find_package(OpenGL REQUIRED)

target_include_directories(EtanolEngine 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/src
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include
		${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include)

set(SYSTEM_LIBS -mwindows ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})