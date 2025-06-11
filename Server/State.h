#pragma once
#include "Bot.h"

class Bot;


class State
{
public:
	virtual void Enter(Bot* bot) = 0;
	virtual void Execute(Bot* bot) = 0;
	virtual void Exit(Bot* bot) = 0;
	virtual ~State() = default;

};

