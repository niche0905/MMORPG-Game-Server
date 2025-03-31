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
	myNP::CS_KEY_INPUT* packet = new myNP::CS_KEY_INPUT{ static_cast<uint8_t>(game.GetID()), static_cast<uint8_t>(c) };
	sf::Socket::Status status = socket.send(packet, sizeof(myNP::CS_KEY_INPUT));
	if (status != sf::Socket::Done) {
		std::wcout << L"Send ����!!!\n";
		getchar();
		exit(-1);
	}

	std::cout << "Send dir: " << static_cast<int>(c) << '\n';
}

std::string Communication::Recv()
{
	char buffer[2];
	std::size_t received;
	sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
	if (status != sf::Socket::Done) {
		std::wcout << L"Recv ����!!!\n";
		getchar();
		exit(-1);
	}

	if (received != 2) {
		std::wcout << L"Recv byte ����!!!\n";
		getchar();
		exit(-1);
	}

	std::string data = "";
	data += buffer[0];
	data += buffer[1];

	std::cout << "Recv x: " << static_cast<int>(data[0]) << ", y :" << static_cast<int>(data[1]) << '\n';

	return data;
}
