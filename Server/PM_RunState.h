#pragma once


class PM_RunState : public State
{
public:
	static PM_RunState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

