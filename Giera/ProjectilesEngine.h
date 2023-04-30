#pragma once
#include "AbstractProjectile.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

using std::thread;

class Board;

class ProjectilesEngine
{
protected:
	weak_ptr <Board> board;
	vector <thread> threads;
	vector <shared_ptr<AbstractProjectile>>* projectiles;
	vector <vector <int> > projIndexesToRemovePerThread;
	int threadsNumber;
	Time timeDiff;
	static inline int minProjPerThread = 50;


	void threadFunction(int id);
public:
	ProjectilesEngine(weak_ptr<Board> board);
	ProjectilesEngine();
	ProjectilesEngine(ProjectilesEngine& pe) = delete;
	void calculateProjectiles(Time timeDiff);

	void setBoard(weak_ptr<Board> board);
};

