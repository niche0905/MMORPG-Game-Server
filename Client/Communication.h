#pragma once

// 네트워크 관련 상수
inline constexpr const char* LOOPBACK_ADDRESS = "127.0.0.1";
constexpr int PORT_NUM{ /*A*/21004 };
constexpr int BUF_SIZE{ 200 };
constexpr int HEADER_SIZE{ 2 };

class MyTcpSocket : public sf::TcpSocket
{
public:
	sf::SocketHandle getHandlePublic();
};


class Communication
{
private:
	MyTcpSocket socket;

	std::vector<char> remain_buffer;

public:
	Communication();

	~Communication();

	void Init();
	void Connect(const char* ip_address);

	void Send(char c);
	std::vector<char> Recv();

	// TODO : Send / Recv + Process 추가하기
	//		  그리고 패킷 설계 하기

};

