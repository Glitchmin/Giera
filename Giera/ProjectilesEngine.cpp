#include "ProjectilesEngine.h"
#include "Board.h"

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

		int start = id * projectiles->size() / threadsNumber;
		int end = id == threadsNumber - 1 ? 
			projectiles->size() :
			(id + 1) * projectiles->size() / threadsNumber;

		Logger::logInfo("thread nr",id,start, end);

		for (int i = start; i < end; i++) {
			(*projectiles)[i]->move(timeDiff);
		}

		int threadsFinished = ++threadsFinishedCounter;
		if (threadsFinished == threadsNumber) {
			allThreadsFinishedCv.notify_all();
		}
		else {
			Logger::logInfo("thread nr", id, "waiting");
			/*std::unique_lock <mutex> lck(allThreadsFinishedMtx);
			while (threadsFinishedCounter.load() != threadsNumber) {
				allThreadsFinishedCv.wait(lck);
			}*/
			Logger::logInfo("thread nr", id, "finished waiting");
		}

	}
}

ProjectilesEngine::ProjectilesEngine(weak_ptr<Board> board)
{
	this->board = board;
	readyToCalculate.store(0);
	threadsFinishedCounter.store(0);

	for (int i = 0; i < threadsNumber; i++) {
		threads.push_back(thread(&ProjectilesEngine::threadFunction, this, i));
	}
	projIndexesToRemovePerThread.resize(threadsNumber);

}

ProjectilesEngine::ProjectilesEngine()
{
	throw "default constructor";
}

ProjectilesEngine::~ProjectilesEngine()
{
	threadsShouldJoin.store(1);
	for (auto& th : threads) {
		th.join();
	}
}

void ProjectilesEngine::calculateProjectiles(Time timeDiff)
{
	auto boardSp = board.lock();
	projectiles = &(boardSp->getProjectiles());
	auto& projectilesToAdd = boardSp->getProjectilesToBeAdded();
	if (!projectilesToAdd.empty()) {
		projectiles->insert(projectiles->begin(), projectilesToAdd.begin(), projectilesToAdd.end());
	}
	projectilesToAdd.clear();
	threadsFinishedCounter.store(0);
	Logger::logInfo("threads starting");
	this->timeDiff = timeDiff;
	readyToCalculate.store(1);
	projShouldBeCalculated.notify_all();

	std::unique_lock <mutex> lck(allThreadsFinishedMtx);
	Logger::logInfo("waiting for threads to finish");
	while (threadsFinishedCounter.load() != threadsNumber) {
		allThreadsFinishedCv.wait(lck);
	}
	Logger::logInfo("threads finished");
	readyToCalculate.store(0);

}

void ProjectilesEngine::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}
