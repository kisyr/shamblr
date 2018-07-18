build:
	mkdir build
	g++ -std=c++14 -o build/shamblr src/main.cpp -lglfw -lGL

clean:
	rm -rf build

