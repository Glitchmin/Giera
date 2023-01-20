#pragma once
#include "AbstractNPC.h"
class Player :
    public AbstractNPC
{
public:
    Player();
    Player(weak_ptr<Board> board);
};

