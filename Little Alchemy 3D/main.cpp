#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

#include "Game.h"

int main(int argc, char* argv[]) {
	Game* game = new Game(argc, argv);
	return 0;
}