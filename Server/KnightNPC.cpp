#include "pch.h"
#include "KnightNPC.h"


KnightNPC::KnightNPC()
	: KnightNPC{ INVALID_ID }
{

}

KnightNPC::KnightNPC(uint64 id)
	: Bot{ id, ClassType::KNIGHT_NPC, false, true, false, true, false }
{

}

KnightNPC::~KnightNPC()
{

}
