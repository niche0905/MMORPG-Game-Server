#include "pch.h"
#include "KnightNPC.h"


KnightNPC::KnightNPC()
	: KnightNPC{ INVALID_ID }
{
}

KnightNPC::KnightNPC(uint64 id)
	: Bot{ id, false, true, false, true, false }
{
}

KnightNPC::~KnightNPC()
{
}
