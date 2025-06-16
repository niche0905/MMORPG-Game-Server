#pragma once


// 비 선공 몬스터에요
class NeutralMonster : public Monster
{
public:
	NeutralMonster();
	NeutralMonster(uint64 id);
	~NeutralMonster();

	bool TakeDamage(uint64 id, uint16 damage) override;

	void Update() override;

	void Attack() override;
	bool Attackalbe(Position pos);

};

