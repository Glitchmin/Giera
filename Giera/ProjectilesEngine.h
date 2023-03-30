#pragma once
#include "AbstractProjectile.h"
#include <thread>


class ProjectilesEngine
{
protected:
	weak_ptr <Board> board;
	vector <std::thread> threads;
	vector <vector <int> > projIndexesToRemovePerThread;
public:
	ProjectilesEngine(weak_ptr<Board> board);
	void calculateProjectiles(Time timeDiff, vector <AbstractProjectile>& projectiles, vector <AbstractProjectile>& projectilesToAdd);
};

