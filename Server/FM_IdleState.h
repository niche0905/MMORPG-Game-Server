#pragma once


class FM_IdleState : public State
{
public:
	static FM_IdleState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

