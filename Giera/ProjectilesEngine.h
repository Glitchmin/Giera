#pragma once
#include "AbstractProjectile.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

using std::atomic;
using std::thread;
using std::mutex;
using std::condition_variable;

class ProjectilesEngine
{
protected:
	weak_ptr <Board> board;
	vector <thread> threads;
	atomic <bool> readyToCalculate;
	atomic <bool> threadsShouldJoin;
	vector <vector <int> > projIndexesToRemovePerThread;
	mutex projCalculationMutex;
	condition_variable projShouldBeCalculated;
	atomic <int> threadsFinishedCounter;
	mutex allThreadsFinishedMtx;
	condition_variable allThreadsFinishedCv;
	int threadsNumber = 4; //TO DO make it depentant on some constant


	void threadFunction(int id);
public:
	ProjectilesEngine(weak_ptr<Board> board);
	ProjectilesEngine(ProjectilesEngine& pe) = delete;
	~ProjectilesEngine();
	void calculateProjectiles(Time timeDiff, vector <AbstractProjectile>& projectiles, vector <AbstractProjectile>& projectilesToAdd);
};

