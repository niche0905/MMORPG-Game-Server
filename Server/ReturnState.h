#pragma once


class ReturnState : public State
{
public:
	static ReturnState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};
