#pragma once
#include "State.h"
#include "Bot.h"

class Bot;
class State;


class FiniteStateMachine
{
private:
	State* _current_state;

public:
	FiniteStateMachine();

	void ChangeState(Bot* bot, State* new_state);
	void Update(Bot* bot);

};

using FSM = FiniteStateMachine;
