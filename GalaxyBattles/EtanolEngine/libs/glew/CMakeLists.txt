cmake_minimum_required(VERSION 2.6)
project("glew")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(RES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res)

set(LIBGLEW_SRCS ${SRC_DIR}/glew.c)

set(DEFINITIONS)
if(WIN32)
    list(APPEND DEFINITIONS -DWIN32_MEAN_AND_LEAN -DVC_EXTRALEAN -D_CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
   # list(APPEND LIBGLEW_SRCS ${RC_DIR}/glew.rc)
endif()

find_package(OpenGL REQUIRED)

if(NOT USE_GLU)
    list(APPEND DEFINITIONS -DGLEW_NO_GLU)
    set(pc_requires)
else()
    set(pc_requires REQUIRES glu)
    list(APPEND LIBRARIES OpenGL::GLU)
    if(NOT OPENGL_GLU_FOUND)
        message(FATAL_ERROR "GLU is not found. but GLU option is enabled")
    endif()
endif( )

if(WIN32)
    list(APPEND LIBRARIES opengl32)
elseif(APPLE)
    find_library(GL_LIBRARY OpenGL)
    find_library(AGL_LIBRARY AGL)
    list(APPEND LIBRARIES ${GL_LIBRARY} ${AGL_LIBRARY})
else()
    list(APPEND LIBRARIES GL X11 dl Xext)
endif()

add_library(libglew_static STATIC ${LIBGLEW_SRCS})

set_target_properties(libglew_static PROPERTIES
                      OUTPUT_NAME "glew"
                      DEBUG_POSTFIX d)
target_include_directories(libglew_static PUBLIC ${INCLUDE_DIR})
target_compile_definitions(libglew_static PUBLIC GLEW_STATIC ${DEFINITIONS})
target_link_libraries(libglew_static ${LIBRARIES} )

