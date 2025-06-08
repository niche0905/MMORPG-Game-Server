#pragma once


class Session : public Creature
{
private:
	SOCKET		_socket;

	ExOver		_recv_overlapped;
	int32		_remain_size;

	// 게임에 필요한 정보 (컨텐츠 관련)
	Stats		_equip_stats = {};		// 장비 스텟(장착한 장비에 따라 증가한 스텟)

	uint64		_last_move_time;

	std::unordered_set<int64>	_view_list;		// 나중에 lock 없는 자료구조 찾아보자
	std::mutex					_view_lock;

public:
	uint64		_ebr_number{ EBR<Session>::MAX_ULLONG };

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	void Disconnect();

	void Reset(SOCKET socket, int64 id);

	bool IsPlayer() const override;
	bool IsNPC() const override;

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// 얘는 private 이여도 될 듯?
	void ReassemblePacket(DWORD recv_size);

	void LoginProcess(BYTE* packet);
	void MoveProcess(BYTE* packet);
	void ChatProcess(BYTE* packet);

	void ProcessCloseCreature(int64 id, void* enter_packet, void* move_packet);
	void SendLeaveCreature(int64 id);

	void SelfUpdate();

};

