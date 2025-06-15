#pragma once


// 비 선공 몬스터에요
class NeutralMonster : public Monster
{
public:
	NeutralMonster();
	NeutralMonster(uint64 id);
	~NeutralMonster();

	void Update() override;
	void DropItem(uint64 id) override;

};

