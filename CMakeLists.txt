cmake_minimum_required(VERSION 3.0)
project(shamblr VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_NO_GLU")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dep/glfw EXCLUDE_FROM_ALL)

add_subdirectory(dep/glew/build/cmake EXCLUDE_FROM_ALL)

add_subdirectory(dep/nanovg EXCLUDE_FROM_ALL)

add_subdirectory(dep/Box2D EXCLUDE_FROM_ALL)

set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(dep/libsndfile EXCLUDE_FROM_ALL)

add_subdirectory(dep/openal-soft EXCLUDE_FROM_ALL)

# Application
add_executable(shamblr 
	src/main.cpp
	src/Game.cpp
	src/service/GraphicsService.cpp
	src/system/HudSystem.cpp
	src/system/PhysicsSystem.cpp
	src/system/PlayerSystem.cpp
	src/system/RenderSystem.cpp
	src/system/SpriteRenderSystem.cpp
	src/system/StrokeSystem.cpp
)
target_include_directories(shamblr PUBLIC dep/glew/include)
target_include_directories(shamblr PUBLIC dep/Box2D)
target_include_directories(shamblr PUBLIC dep/nanovg/src)
target_include_directories(shamblr PUBLIC dep/openal-soft/include)
target_include_directories(shamblr PUBLIC dep/include)
target_link_libraries(shamblr glfw glew_s nanovg Box2D sndfile-static OpenAL -static-libgcc -static-libstdc++)

# Installation
install(TARGETS shamblr RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin")
