#include "pch.h"
#include "PM_ReturnState.h"


PM_ReturnState& PM_ReturnState::Instance()
{
	static PM_ReturnState instance;
	return instance;
}

void PM_ReturnState::Enter(Bot* bot)
{
	// TODO: 경로 추적하고 해당 경로 설정 하여야 한다
}

void PM_ReturnState::Execute(Bot* bot)
{
	
}

void PM_ReturnState::Exit(Bot* bot)
{

}
