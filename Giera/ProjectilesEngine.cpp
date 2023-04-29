#include "ProjectilesEngine.h"
#include "Board.h"
#include <algorithm>

using std::for_each;
using std::unique_lock;

void ProjectilesEngine::threadFunction(int id)
{
	Logger::logInfo("thread", id, "starting");

	int start = id * projectiles->size() / threadsNumber;
	int end = id == threadsNumber - 1 ?
		projectiles->size() :
		(id + 1) * projectiles->size() / threadsNumber;

	Logger::logInfo("thread nr", id, start, end);

	for (int i = start; i < end; i++) {
		(*projectiles)[i]->move(timeDiff);
	}

}

ProjectilesEngine::ProjectilesEngine(weak_ptr<Board> board)
{
	this->board = board;

}

ProjectilesEngine::ProjectilesEngine()
{
	throw "default constructor";
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
	this->timeDiff = timeDiff;
	threads.clear();
	projIndexesToRemovePerThread.clear();
	threadsNumber = thread::hardware_concurrency() == 0 ? 4 : thread::hardware_concurrency();
	threadsNumber = std::min((int)projectiles->size() / minProjPerThread, threadsNumber);
	threadsNumber += (threadsNumber == 0);
	projIndexesToRemovePerThread.resize(threadsNumber);
	for (int i = 0; i < threadsNumber;i++) {
		threads.emplace_back(&ProjectilesEngine::threadFunction, this, i);
	}
	for (auto& th : threads) {
		th.join();
	}
}

void ProjectilesEngine::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}
