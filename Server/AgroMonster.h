#pragma once


// 선공 몬스터에요
class AgroMonster : public Monster
{
public:
	AgroMonster();
	AgroMonster(uint64 id);
	~AgroMonster();

	void Update() override;

	void Attack() override;
	bool Attackalbe(Position pos);

};

