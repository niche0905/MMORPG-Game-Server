#include "pch.h"
#include "ReturnState.h"


ReturnState& ReturnState::Instance()
{
	static ReturnState instance;
	return instance;
}

void ReturnState::Enter(Bot* bot)
{
	std::vector<Position> path = Astar::FindPath(bot->GetPosition(), bot->GetBasePosition(), [](const Position& pos) { return server.IsBlock(pos); });
	bot->SetPath(std::move(path));
}

void ReturnState::Execute(Bot* bot)
{
	bot->DoPathFollow();
}

void ReturnState::Exit(Bot* bot)
{

}
