#pragma once


class PM_ReturnState : public State
{
public:
	static PM_ReturnState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

