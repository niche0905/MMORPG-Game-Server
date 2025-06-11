#pragma once


class Bot : public Creature
{
private:
	std::atomic_bool		_is_active = false;		// 주위에 플레이어가 있어서 활동 중인가?

public:
	Bot();
	Bot(uint64 id);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	uint16 GetMaxHP() const override;

	void WakeUp();

	void DoRandomMove();

};

