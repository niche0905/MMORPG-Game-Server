#pragma once

// ��Ʈ��ũ ���� ���
inline constexpr const char* LOOPBACK_ADDRESS = "127.0.0.1";
constexpr int PORT_NUM{ /*A*/21004 };
constexpr int BUF_SIZE{ 200 };


class Communication
{
private:
	sf::TcpSocket socket;

public:
	Communication();

	~Communication();

	void Init();
	void Connect(const char* ip_address);

	void Send(char c);
	std::string Recv();

	// TODO : Send / Recv + Process �߰��ϱ�
	//		  �׸��� ��Ŷ ���� �ϱ�

};

