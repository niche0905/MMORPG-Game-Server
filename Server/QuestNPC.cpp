#include "pch.h"
#include "QuestNPC.h"


QuestNPC::QuestNPC()
	: QuestNPC{ INVALID_ID }
{

}

QuestNPC::QuestNPC(uint64 id)
	: Bot{ id, Bot::BotType::QUEST_NPC, true, true, true, true, true }
{

}

QuestNPC::~QuestNPC()
{

}
