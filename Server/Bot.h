#pragma once


class Bot : public Creature
{
private:


public:
	Bot();
	Bot(int64 id);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

};

