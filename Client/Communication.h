#pragma once


class MyTcpSocket : public sf::TcpSocket
{
public:
	sf::SocketHandle getHandlePublic();
};


class Communication
{
private:
	MyTcpSocket socket;

	std::vector<BYTE> remain_buffer;

public:
	Communication();

	~Communication();

	void Init();
	void Connect(const char* ip_address);

	void Send(BYTE* buffer, uint16 len);
	std::vector<BYTE> Recv();

};

