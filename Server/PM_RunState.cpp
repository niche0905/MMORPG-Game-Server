#include "pch.h"
#include "PM_RunState.h"


PM_RunState& PM_RunState::Instance()
{
	static PM_RunState instance;
	return instance;
}

void PM_RunState::Enter(Bot* bot)
{

}

void PM_RunState::Execute(Bot* bot)
{
	// TODO: 적 반대 방향으로 도망치기
}

void PM_RunState::Exit(Bot* bot)
{

}
