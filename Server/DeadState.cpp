#include "pch.h"
#include "DeadState.h"


DeadState& DeadState::Instance()
{
	static DeadState instance;
	return instance;
}

void DeadState::Enter(Bot* bot)
{

}

void DeadState::Execute(Bot* bot)
{

}

void DeadState::Exit(Bot* bot)
{
	// TODO: 사망 종료 시 패널티 ㄱㄱ
}
