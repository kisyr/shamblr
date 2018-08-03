#include "Game.hpp"

using namespace shamblr;

int main(int argc, char* argv[]) {
	Game game(glm::ivec2(1280, 768));
	game.run();

	return EXIT_SUCCESS;
}
