#pragma once


class KnightNPC : public Bot
{
public:
	KnightNPC();
	KnightNPC(uint64 id);
	~KnightNPC();

	void Update() override;

	void Attack() override;
	bool Attackalbe(Position pos);

};

