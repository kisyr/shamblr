SOURCES = $(wildcard src/*.cpp) $(wildcard src/service/*.cpp) $(wildcard src/system/*.cpp) $(wildcard src/*.c)

build-linux:
	mkdir build
	g++ -std=c++14 -o build/shamblr -Ilib/include $(SOURCES) -lglfw -lBox2D -lGL -lGLEW

build-windows:
	mkdir build
	x86_64-w64-mingw32-g++ -static -static-libgcc -static-libstdc++ -std=c++14 -o build/shamblr.exe -Ilib/include $(SOURCES) lib/windows/libglfw3.a lib/windows/libBox2D.a -mwindows -lopengl32

clean:
	rm -rf build

