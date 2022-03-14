#include "GameMaster.h"

int main()
{
	GameMaster gm;
	bool init = gm.Initialize();
	if (init) gm.GameLoop();

	return 0;
}