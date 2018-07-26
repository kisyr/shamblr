SOURCES = $(wildcard src/*.cpp) $(wildcard src/system/*.cpp)

build:
	mkdir build
	g++ -std=c++14 -o build/shamblr $(SOURCES) -lglfw -lGL -lBox2D

clean:
	rm -rf build

