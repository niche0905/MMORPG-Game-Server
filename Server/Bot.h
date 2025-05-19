#pragma once


class Bot : public BaseSession
{
private:


public:
	Bot();
	Bot(int64 id);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

};

