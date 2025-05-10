#pragma once

class Session
{
private:
	SOCKET		_socket;
	int64		_id;

	ExOver		_recv_overlapped;
	int32		_remain_size;

	// ���ӿ� �ʿ��� ���� (������ ����)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// ��� private �̿��� �� ��?
	void ReassemblePacket(BYTE* recv_data, DWORD recv_size);

};

