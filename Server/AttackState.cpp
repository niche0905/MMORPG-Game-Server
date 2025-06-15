#include "pch.h"
#include "AttackState.h"


AttackState& AttackState::Instance()
{
	static AttackState instance;
	return instance;
}

void AttackState::Enter(Bot* bot)
{

}

void AttackState::Execute(Bot* bot)
{
	bot->Attack();
}

void AttackState::Exit(Bot* bot)
{

}
