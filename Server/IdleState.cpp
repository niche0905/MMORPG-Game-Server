#include "pch.h"
#include "IdleState.h"


IdleState& IdleState::Instance()
{
	static IdleState instance;
	return instance;
}

void IdleState::Enter(Bot* bot)
{

}

void IdleState::Execute(Bot* bot)
{
	bot->DoRandomMove();
}

void IdleState::Exit(Bot* bot)
{

}
