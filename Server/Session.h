#pragma once


class Session : public Creature
{
private:
	using time_point = std::chrono::steady_clock::time_point;

private:
	// 게임에 필요한 정보 (컨텐츠 관련)
	
	uint64		_exp = 0;
	Stats		_equip_stats = {};		// 장비 스텟(장착한 장비에 따라 증가한 스텟)

	uint64		_last_move_time = 0;

	std::unordered_set<uint64>	_view_list;		// 나중에 lock 없는 자료구조 찾아보자
	std::mutex					_view_lock;

	uint64		_user_id = INVALID_ID;

	time_point	_move_cooltime;
	time_point	_aatk_cooltime;
	time_point	_satk_cooltime;
	time_point	_datk_cooltime;

	// 네트워크 관련

	SOCKET		_socket;

	ExOver		_recv_overlapped;
	int32		_remain_size;
	Position	_base_pos = { 398, 786 };	// TEMP

	std::atomic_bool _is_timed = false;		// 타이머가 걸렸는지

public:
	uint64		_ebr_number{ EBR<Session>::MAX_ULLONG };

public:
	Session();
	Session(SOCKET socket, uint64 id);

	~Session();

	void Disconnect();

	void Reset(SOCKET socket, uint64 id);

	bool IsPlayer() const override;
	bool IsNPC() const override;

	uint64 GetExp() const;

	Stats GetStats() const override;
	Stats GetEquipStats() const;

	bool TakeDamage(uint64 id, uint16 damage) override;
	void DeadSequence() override;

	void UpdateVisualInfo();
	void SetClassType(uint8 class_type) override;

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);
	void ReassemblePacket(DWORD recv_size);

	void LoginProcess(BYTE* packet);
	void RegisterProcess(BYTE* packet);
	void MoveProcess(BYTE* packet);
	void ChatProcess(BYTE* packet);
	void AttackProcess(BYTE* packet);
	void TeleportProcess(BYTE* packet);
	void RespawnProcess(BYTE* packet);
	void InteractionProcess(BYTE* packet);
	void SetBasePosProcess(BYTE* packet);

	uint64 GetUserID() const;
	bool LoginInfo(uint64 user_id, int16 x, int16 y, uint16 maxHP, uint16 HP, uint8 class_type, uint32 level, uint64 exp);
	void LoginDone();
	void LoginFalse();

	bool RegisterInfo(uint64 user_id);
	void RegisterFalse();

	void ProcessCloseCreature(uint64 id, void* enter_packet, void* move_packet, void* vi_packet = nullptr);
	void SendNewCreature(uint64 id, void* enter_packet);
	void SendLeaveCreature(uint64 id);

	void HealSelf10();
	void AddExp(uint64 exp);

private:
	void SelfUpdate();
	void StatsChange();
	void NeedGeneration();

};

