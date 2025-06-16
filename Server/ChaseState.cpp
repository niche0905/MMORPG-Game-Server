#include "pch.h"
#include "ChaseState.h"

ChaseState& ChaseState::Instance()
{
	static ChaseState instance;
	return instance;
}

void ChaseState::Enter(Bot* bot)
{
	Creature* target = bot->GetTarget();
	if (target == nullptr) return;

	std::vector<Position> path = Astar::FindPath(bot->GetPosition(), target->GetPosition(), [](const Position& pos) { return server.IsBlock(pos); });
	bot->SetPath(std::move(path));
}

void ChaseState::Execute(Bot* bot)
{
	bot->DoPathFollow();
}

void ChaseState::Exit(Bot* bot)
{

}
