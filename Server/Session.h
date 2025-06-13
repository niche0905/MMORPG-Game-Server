#pragma once


class Session : public Creature
{
private:
	// 게임에 필요한 정보 (컨텐츠 관련)
	
	uint64		_exp = 0;
	Stats		_equip_stats = {};		// 장비 스텟(장착한 장비에 따라 증가한 스텟)

	uint64		_last_move_time = 0;

	std::unordered_set<uint64>	_view_list;		// 나중에 lock 없는 자료구조 찾아보자
	std::mutex					_view_lock;

	uint8		_class_type = 0;			// 직업 정보

	// 네트워크 관련

	SOCKET		_socket;

	ExOver		_recv_overlapped;
	int32		_remain_size;

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

	uint16 GetMaxHP() const override;
	void TakeDamage(uint16 damage);
	void DeadSequence() override;

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// 얘는 private 이여도 될 듯?
	void ReassemblePacket(DWORD recv_size);

	void LoginProcess(BYTE* packet);
	void MoveProcess(BYTE* packet);
	void ChatProcess(BYTE* packet);
	void AttackProcess(BYTE* packet);

	void ProcessCloseCreature(uint64 id, void* enter_packet, void* move_packet);
	void SendNewCreature(uint64 id, void* enter_packet);
	void SendLeaveCreature(uint64 id);

	void SelfUpdate();

};

