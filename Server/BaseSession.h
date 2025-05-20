#pragma once


class Creature
{
protected:
	int64		_id;

	// User와 NPC 모두 가지는 공통 Game Contents를 담는 변수
	Stats		_basic_stats;		// 기본 스텟(레벨에 따른 스텟)
	Stats		_temp_stats;		// 임시 스텟(버프, 디버프)

public:
	Creature();
	Creature(int64 id);
	~Creature();

	// 필요한 메서드
	int64 GetID() const;
	virtual bool IsPlayer() const = 0;
	virtual bool IsNPC() const = 0;

};

