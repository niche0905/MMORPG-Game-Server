#pragma once
#include "FiniteStateMachine.h"


class Bot : public Creature
{
protected:
	std::atomic_bool		_is_active = false;		// 주위에 플레이어가 있어서 활동 중인가?

	FSM						_fsm;

	bool					_is_invincibility;		// 무적인지 (상호 공격 불가 몬스터)
	bool					_is_neutrality;			// 중립 몹인지 (선공을 하지 않고 피격 시)
	bool					_is_peace;				// 공격을 하지 않는 몹인지
	bool					_is_fix;				// 고정 몹인지 (가만히 만 있는지)

	// TODO: 필요 시 더 추가할 것

public:
	Bot();
	Bot(uint64 id);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	uint16 GetMaxHP() const override;

	void WakeUp();

	void DoRandomMove();

};

