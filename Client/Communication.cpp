#include "pch.h"
#include "Communication.h"


// �⺻ ������ local loop�� ����
Communication::Communication()
{
	Init();
}

// �־��� �ּҷ� ����
Communication::Communication(const char* ip_address)
{
	Init(ip_address);
}

// �Ҹ��ڷ� ���� �ʿ��� �׸� ����
Communication::~Communication()
{

}

// �ʱ⿡ �ʿ��� �۾��� ex) ����ŷ ����
void Communication::Init(const char* ip_address)
{
	Connect(ip_address);		// �ּҿ� ����

	socket.setBlocking(false);	// ���� ����� ����ŷ���� ����
}

// �ּҿ� ��� ����
void Communication::Connect(const char* ip_address)
{
	sf::Socket::Status status = socket.connect(ip_address, PORT_NUM);
	if (status != sf::Socket::Done) {
		std::wcout << L"������ ���� �� �� �����ϴ�. ���α׷��� ���� �� �ٽ� �õ��� �ּ���\n";
		getchar();
	}
}
