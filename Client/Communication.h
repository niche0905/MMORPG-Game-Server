#pragma once

// 네트워크 관련 상수
const char* IP_ADDRESS{ "127.0.0.1" };
constexpr int PORT_NUM{ /*A*/21004 };
constexpr int BUF_SIZE{ 200 };


class Communication
{
private:
	sf::TcpSocket socket;

public:
	Communication();
	Communication(const char* ip_addrsss);

	~Communication();

	void Init(const char* ip_address = IP_ADDRESS);
	void Connect(const char* ip_address);

	// TODO : Send / Recv + Process 추가하기
	//		  그리고 패킷 설계 하기

};

