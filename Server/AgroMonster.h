#pragma once


// 선공 몬스터에요
class AgroMonster : public Monster
{
	AgroMonster();
	AgroMonster(uint64 id);
	~AgroMonster();

	void Update() override;
	void DropItem(uint64 id) override;

};

