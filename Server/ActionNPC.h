#pragma once


class ActionNPC : public Bot
{
public:
	ActionNPC();
	ActionNPC(uint64 id);
	~ActionNPC();

	void Update() override;

	virtual void DoAction(uint64 id);
	virtual bool CanAction(Position pos);

};

