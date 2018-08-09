#include "Game.hpp"

using namespace shamblr;

int main(int argc, char* argv[]) {
	Game game(glm::ivec2(1920, 1024));
	game.run();

	return EXIT_SUCCESS;
}
