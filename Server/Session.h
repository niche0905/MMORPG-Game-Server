#pragma once


class Session : public Creature
{
private:
	SOCKET		_socket;

	ExOver		_recv_overlapped;
	int32		_remain_size;

	// 게임에 필요한 정보 (컨텐츠 관련)
	Stats		_equip_stats;		// 장비 스텟(장착한 장비에 따라 증가한 스텟)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// 얘는 private 이여도 될 듯?
	void ReassemblePacket(DWORD recv_size);

};

