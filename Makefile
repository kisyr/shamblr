SOURCES = $(wildcard src/*.cpp) $(wildcard src/system/*.cpp)

build-linux:
	mkdir build
	g++ -std=c++14 -o build/shamblr -I./include $(SOURCES) lib/linux/libBox2D.a -lglfw -lGL

build-windows:
	mkdir build
	x86_64-w64-mingw32-g++ -static -static-libgcc -static-libstdc++ -std=c++14 -o build/shamblr.exe -Ilib/include $(SOURCES) lib/windows/libglfw3.a lib/windows/libBox2D.a -mwindows -lopengl32

clean:
	rm -rf build

