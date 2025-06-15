#pragma once


// 고정된 몬스터 (비선공)
class FixedMonster : public Monster
{
	FixedMonster();
	FixedMonster(uint64 id);
	~FixedMonster();

	void Update() override;
	void DropItem(uint64 id) override;

};

