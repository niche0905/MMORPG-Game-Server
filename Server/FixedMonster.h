#pragma once


// 고정된 몬스터 (비선공)
class FixedMonster : public Monster
{
public:
	FixedMonster();
	FixedMonster(uint64 id);
	~FixedMonster();

	bool TakeDamage(uint64 id, uint16 damage) override;

	void Update() override;

	void Attack() override;

	void ReviveChangeState() override;

};

