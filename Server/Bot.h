#pragma once
#include "FiniteStateMachine.h"


class Bot : public Creature
{
protected:
	FSM						_fsm;

	Creature*				_target;						// 공격 대상
	std::vector<Position>	_path;							// 가야할 경로 (_target이 유효할 때만 사용할 것)
	uint64					_current_index = -1;			// 현재 경로 진행 정도

	Position				_base_pos = { 0, 0 };			// 기본 위치

	bool					_is_invincibility = false;		// 무적인지 (상호 공격 불가 몬스터)
	bool					_is_neutrality = false;			// 중립 몹인지 (선공을 하지 않고 피격 시)
	bool					_is_peace = false;				// 공격을 하지 않는 몹인지
	bool					_is_friendly = false;			// 우호적인지 (Player와 전투가 성립되지 않음)
	bool					_is_fix = false;				// 고정 몹인지 (가만히 만 있는지)
	bool					_is_action = false;				// 상호작용이 가능한지

	std::atomic_bool		_is_active = false;		// 주위에 플레이어가 있어서 활동 중인가?

	// TODO: 필요 시 더 추가할 것

public:
	Bot();
	Bot(uint64 id);
	Bot(uint64 id, uint8 b_type, bool invin, bool neut, bool peace, bool firend, bool fix);
	~Bot();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	bool TakeDamage(uint64 id, uint16 damage) override;
	void DeadSequence() override;

	uint8 GetBotType() const;

	bool IsInvincibility() const;
	bool IsNeutrality() const;
	bool IsPeace() const;
	bool IsFriendly() const;
	bool IsFix() const;
	bool IsAction() const;

	virtual void Update();

	virtual void FirstSeen(uint64 id);
	void WakeUp();
	bool ClosePlayer();
	void DisableActive();

	virtual void Attack();

	bool DoMove(const Position& pos);
	void DoRandomMove();
	void DoRevive() override;
	virtual void ReviveInit();
	virtual void ReviveChangeState();
	void DoPathFollow();

	void SetPath(const std::vector<Position>& path);
	void SetPath(std::vector<Position>&& path);

	void SetBasePosition(int16 x, int16 y);
	void SetBasePosition(const Position& pos);
	Position GetBasePosition() const;

	Creature* GetTarget() const;

	void AttackBroadcast(void* attack_packet);
	
	void DropItem(uint64 id);
	virtual uint64 GetDropExp();

};

