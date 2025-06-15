#pragma once


// 도망만 가는 몬스터에요
class PeaceMonster : public Monster
{
	PeaceMonster();
	PeaceMonster(uint64 id);
	~PeaceMonster();

	void Update() override;
	void DropItem(uint64 id) override;

};

