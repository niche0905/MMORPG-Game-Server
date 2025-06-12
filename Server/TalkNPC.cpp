#include "pch.h"
#include "TalkNPC.h"


TalkNPC::TalkNPC()
	: TalkNPC{ INVALID_ID }
{

}

TalkNPC::TalkNPC(uint64 id)
	: Bot{id, Bot::BotType::TALK_NPC, true, true, true, true, true}
{

}

TalkNPC::~TalkNPC()
{

}
