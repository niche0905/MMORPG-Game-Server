#include "pch.h"
#include "PM_ReturnState.h"


PM_ReturnState& PM_ReturnState::Instance()
{
	static PM_ReturnState instance;
	return instance;
}

void PM_ReturnState::Enter(Bot* bot)
{

}

void PM_ReturnState::Execute(Bot* bot)
{
	// TODO: 길찾기 알고리즘으로 Base_pos로 돌아가기
}

void PM_ReturnState::Exit(Bot* bot)
{

}
