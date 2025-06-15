#include "pch.h"
#include "PM_ReturnState.h"


PM_ReturnState& PM_ReturnState::Instance()
{
	static PM_ReturnState instance;
	return instance;
}

void PM_ReturnState::Enter(Bot* bot)
{
	std::vector<Position> path = Astar::FindPath(bot->GetPosition(), bot->GetBasePosition(), [](const Position& pos) { return server.IsBlock(pos); });
	bot->SetPath(std::move(path));
}

void PM_ReturnState::Execute(Bot* bot)
{
	
}

void PM_ReturnState::Exit(Bot* bot)
{

}
