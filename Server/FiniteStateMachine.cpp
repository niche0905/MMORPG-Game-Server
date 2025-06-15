#include "pch.h"
#include "FiniteStateMachine.h"


FiniteStateMachine::FiniteStateMachine()
	: _current_state{ nullptr }
{

}

void FiniteStateMachine::ChangeState(Bot* bot, State* new_state)
{
	if (_current_state == new_state) return;	// 쓸데 없는 호출 방지

	if (_current_state)
		_current_state->Exit(bot);

	_current_state = new_state;

	if (_current_state)
		_current_state->Enter(bot);
}

void FiniteStateMachine::ForceChangeState(Bot* bot, State* new_state)
{
	if (_current_state)
		_current_state->Exit(bot);

	_current_state = new_state;

	if (_current_state)
		_current_state->Enter(bot);
}

void FiniteStateMachine::Update(Bot* bot)
{
	if (_current_state)
		_current_state->Execute(bot);
}
