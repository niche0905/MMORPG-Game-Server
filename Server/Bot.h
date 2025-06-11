#pragma once
#include "FiniteStateMachine.h"


class Bot : public Creature
{
protected:
	FSM						_fsm;

	bool					_is_invincibility = false;		// 무적인지 (상호 공격 불가 몬스터)
	bool					_is_neutrality = false;			// 중립 몹인지 (선공을 하지 않고 피격 시)
	bool					_is_peace = false;;				// 공격을 하지 않는 몹인지
	bool					_is_fix = false;;				// 고정 몹인지 (가만히 만 있는지)

	std::atomic_bool		_is_active = false;		// 주위에 플레이어가 있어서 활동 중인가?

	// TODO: 필요 시 더 추가할 것

public:
	Bot();
	Bot(uint64 id);
	Bot(uint64 id, bool invin, bool neut, bool peace, bool fix);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	uint16 GetMaxHP() const override;

	bool GetInvincibility();
	bool GetNeutrality();
	bool GetPeace();
	bool GetFix();

	void WakeUp();

	void DoRandomMove();

};

