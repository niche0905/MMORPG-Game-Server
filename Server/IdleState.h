#pragma once


class IdleState : public State
{
public:
	static IdleState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

