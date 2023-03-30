#include "ProjectilesEngine.h"

ProjectilesEngine::ProjectilesEngine(weak_ptr<Board> board)
{
	this->board = board;
	threads.resize(4); //TO DO make it depentant on some constant

}
