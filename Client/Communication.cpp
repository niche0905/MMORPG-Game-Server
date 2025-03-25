#include "pch.h"
#include "Communication.h"


// �⺻ ������
Communication::Communication()
{
	Init();
}

// �Ҹ��ڷ� ���� �ʿ��� �׸� ����
Communication::~Communication()
{

}

// �ʱ⿡ �ʿ��� �۾��� ex) ����ŷ ����
void Communication::Init()
{
	//socket.setBlocking(false);	// ���� ����� ����ŷ���� ����
}

// �ּҿ� ��� ����
void Communication::Connect(const char* ip_address)
{
	sf::Socket::Status status = socket.connect(ip_address, PORT_NUM);
	if (status != sf::Socket::Done) {
		std::wcout << L"������ ���� �� �� �����ϴ�. ���α׷��� ���� �� �ٽ� �õ��� �ּ���\n";
		getchar();
		exit(-1);
	}
}

void Communication::Send(char c)
{
	sf::Socket::Status status = socket.send(&c, 1);
	if (status != sf::Socket::Done) {
		std::wcout << L"Send ����!!!\n";
		getchar();
		exit(-1);
	}

	std::cout << "Send dir: " << static_cast<int>(c) << '\n';
}

std::string Communication::Recv()
{
	sf::Packet packet;
	sf::Socket::Status status = socket.receive(packet);
	if (status != sf::Socket::Done) {
		std::wcout << L"Recv ����!!!\n";
		getchar();
		exit(-1);
	}

	std::string data;
	packet >> data;

	std::cout << "Recv x: " << static_cast<int>(data[0]) << ", y :" << static_cast<int>(data[1]) << '\n';

	return data;
}
