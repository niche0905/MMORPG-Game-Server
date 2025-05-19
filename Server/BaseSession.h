#pragma once


class BaseSession
{
protected:
	int64		_id;

	// User와 NPC 모두 가지는 공통 Game Contents를 담는 변수
	Stats		_basic_stats;
	Stats		_temp_stats;

public:
	BaseSession();
	BaseSession(int64 id);
	~BaseSession();

	// 필요한 메서드
	int64 GetID() const;
	virtual bool IsPlayer() const = 0;
	virtual bool IsNPC() const = 0;

};

