#include "pch.h"
#include "PM_IdleState.h"


PM_IdleState& PM_IdleState::Instance()
{
	static PM_IdleState instance;
	return instance;
}

void PM_IdleState::Enter(Bot* bot)
{

}

void PM_IdleState::Execute(Bot* bot)
{
	bot->DoRandomMove();
}

void PM_IdleState::Exit(Bot* bot)
{

}
