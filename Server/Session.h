#pragma once

class Session
{
private:
	SOCKET		_socket;
	int64		_id;

	ExOver		_recv_overlapped;
	int32		_remain_size;

	// 게임에 필요한 정보 (컨텐츠 관련)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// 얘는 private 이여도 될 듯?
	void ReassemblePacket(BYTE* recv_data, DWORD recv_size);

};

