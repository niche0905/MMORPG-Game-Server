#pragma once


// 도망만 가는 몬스터에요
class PeaceMonster : public Monster
{
public:
	PeaceMonster();
	PeaceMonster(uint64 id);
	~PeaceMonster();

	bool TakeDamage(uint64 id, uint16 damage) override;

	void Update() override;

	void ReviveChangeState() override;

};

