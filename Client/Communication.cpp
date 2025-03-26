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
	char buffer[1];
	buffer[0] = c;
	sf::Socket::Status status = socket.send(buffer, 1);
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
