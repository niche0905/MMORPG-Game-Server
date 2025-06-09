#pragma once


class Bot : public Creature
{
private:


public:
	Bot();
	Bot(uint64 id);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

};

