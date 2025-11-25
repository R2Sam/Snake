#include "Engine/Game.h"

#include "FirstScene.h"

int main ()
{
	Game game(900, 900, "Snake");

	game.SetFirstScene<FirstScene>("First");

	game.Run(60);

	return 0;
}