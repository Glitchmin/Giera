#include "ProjectilesEngine.h"

void ProjectilesEngine::threadFunction(int id)
{
	while (1) {
		std::unique_lock <mutex> lck(projCalculationMutex);
		while (!readyToCalculate.load()) {
			projShouldBeCalculated.wait(lck);
		}
		if (threadsShouldJoin.load()) {
			return;
		}

		Logger::logInfo(id);

		int threadsFinished = ++threadsFinishedCounter;
		if (threadsFinished == threadsNumber) {
			allThreadsFinishedCv.notify_one();
		}

	}
}

ProjectilesEngine::ProjectilesEngine(weak_ptr<Board> board)
{
	this->board = board;
	readyToCalculate.store(0);
	threadsFinishedCounter.store(0);

	for (int i = 0; i < threadsNumber; i++) {
		threads.push_back(thread(&ProjectilesEngine::threadFunction,this, i));
	}
	projIndexesToRemovePerThread.resize(threadsNumber);

}

ProjectilesEngine::~ProjectilesEngine()
{
	threadsShouldJoin.store(1);
	for (auto& th : threads) {
		th.join();
	}
}

void ProjectilesEngine::calculateProjectiles(Time timeDiff, vector<AbstractProjectile>& projectiles, vector<AbstractProjectile>& projectilesToAdd)
{
	auto boardSp = board.lock();
	threadsFinishedCounter.store(0);
	readyToCalculate.store(1);
	projShouldBeCalculated.notify_all();

	std::unique_lock <mutex> lck(allThreadsFinishedMtx);
	allThreadsFinishedCv.wait(lck);
	
}
