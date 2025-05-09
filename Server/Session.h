#pragma once

class Session
{
private:
	SOCKET		_socket;
	int64		_id;

	// ���ӿ� �ʿ��� ���� (������ ����)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);

};

