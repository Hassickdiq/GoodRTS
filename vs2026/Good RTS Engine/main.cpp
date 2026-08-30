#include "main.hpp"
#include "gamesys.hpp"

using namespace std;

int main() {
	Game* mainGame = new Game();
	mainGame->GameLoop();
	return 0;
}