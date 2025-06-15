#pragma once


class AttackState : public State
{
public:
	static AttackState& Instance();

	void Enter(Bot* bot) override;
	void Execute(Bot* bot) override;
	void Exit(Bot* bot) override;

};

