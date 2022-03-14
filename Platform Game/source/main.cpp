#include "state machine/Game.h"

int main(int argc, char* argv[])
{
	Game::GetInstance().Initialize();
	Game::GetInstance().Run();	

	return 0;
}