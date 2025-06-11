#pragma once


class DeadState : public State
{
public:
	static DeadState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

