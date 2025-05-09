#pragma once

class Session
{
private:
	SOCKET		_socket;
	int64		_id;

	// 게임에 필요한 정보 (컨텐츠 관련)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);

};

