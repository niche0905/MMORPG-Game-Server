#pragma once


class ChaseState : public State
{
public:
	static ChaseState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

