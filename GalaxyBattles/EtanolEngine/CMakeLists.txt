add_library(EtanolEngine STATIC src/Engine.hpp src/Engine.cpp  )

#SDL
if(NOT IOS)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2)
	set(SDL2MAIN_LIBRARY SDL2main)
	set(SDL2_LIBRARY SDL2-static)
endif()

target_link_libraries(EtanolEngine PUBLIC
	${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY})

target_include_directories(EtanolEngine 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/src
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2/include)