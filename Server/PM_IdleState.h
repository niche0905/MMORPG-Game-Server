#pragma once


class PM_IdleState : public State
{
public:
	static PM_IdleState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

