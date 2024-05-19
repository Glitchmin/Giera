#include "pch.h"

#include "ProjectilesEngine.h"
#include "Board.h"


void ProjectilesEngine::threadFunction(int id, shared_ptr<Board> board)
{

	size_t start = id * projectiles->size() / threadsNumber;
	size_t end = id == threadsNumber - 1 ?
		projectiles->size() :
		(id + 1) * projectiles->size() / threadsNumber;

	//Logger::logInfo("thread nr", id, start, end);

	for (int i = start; i < end; i++) {
		(*projectiles)[i]->move(timeDiff, board);
		if ((*projectiles)[i]->canBeRemoved()) {
			projIndexesToRemovePerThread[id].push_back(i);
		}
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
	this->timeDiff = timeDiff;
	threads.clear();
	projIndexesToRemovePerThread.clear();
	threadsNumber = thread::hardware_concurrency() == 0 ? 4 : thread::hardware_concurrency();
	threadsNumber = std::min((int)projectiles->size() / minProjPerThread, threadsNumber);
	threadsNumber += (threadsNumber == 0);
	projIndexesToRemovePerThread.resize(threadsNumber);

	for (int i = 0; i < threadsNumber;i++) {
		threads.emplace_back(&ProjectilesEngine::threadFunction, this, i, boardSp);
	}
	for (auto& th : threads) {
		th.join();
	}
	for (auto indexes = projIndexesToRemovePerThread.rbegin();
		indexes != projIndexesToRemovePerThread.rend(); ++indexes) {
		for (auto it = indexes->rbegin(); it != indexes->rend(); ++it) {
			boardSp->removeProjectile(*it);
		}
	}
}

void ProjectilesEngine::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}
