set(CMAKE_CXX_STANDARD 17)

add_library(EtanolEngine STATIC src/Engine.hpp src/Engine.cpp "src/Window.hpp" "src/SDL/SDLWindow.hpp" "src/SDL/SDLWindow.cpp" "src/Renderer.hpp" "src/SDL/SDLRenderer.hpp" "src/SDL/SDLRenderer.cpp" "src/Renderer.cpp" "src/GL/GLRenderer.hpp" "src/GL/GLRenderer.cpp" "src/Bitmap.hpp" "src/Bitmap.cpp" "src/ShaderProgram.hpp" "src/MeshData.hpp" "src/GL/GLVertexBuffer.hpp" "src/VertexBuffer.hpp" "src/VertexBuffer.cpp" "src/GL/GLVertexBuffer.cpp" "src/GL/GLProgram.hpp" "src/GL/GLProgram.cpp" "src/GL/GLTexture.hpp" "src/GL/GLTexture.cpp" "src/Texture.hpp" "src/Texture.cpp" "src/GL/GLDrawProgram.hpp" "src/GL/GLDrawProgram.cpp" "src/Sound.hpp" "src/Sound.cpp" "src/EventsManager.cpp" "src/EventsManager.hpp" "src/Node.hpp" "src/Node.cpp" "src/AudioManager.hpp" "src/AudioManager.cpp" "src/Sprite.cpp" "src/Sprite.hpp" "src/Button.hpp" "src/Button.cpp" "src/UIManager.hpp" "src/UIManager.cpp")


#SDL
if(NOT IOS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
	set(SDL2MAIN_LIBRARY SDL2main)
	set(SDL2_LIBRARY SDL2-static)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glew)
set(GLEW_LIBRARY libglew_static)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
set(IMGUI_LIBRARY imgui)


target_link_libraries(EtanolEngine PUBLIC
	${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY} ${GLEW_LIBRARY} ${IMGUI_LIBRARY})

find_package(OpenGL REQUIRED)

target_include_directories(EtanolEngine 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/src
		${CMAKE_CURRENT_SOURCE_DIR}/libs
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include
		${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include
		)

set(SYSTEM_LIBS -mwindows ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})