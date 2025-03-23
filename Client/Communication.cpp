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
	std::wcout.imbue(std::locale("korean"));			// ǥ�� ��� ��Ʈ���� ����

	socket.setBlocking(false);	// ���� ����� ����ŷ���� ����

	std::wcout << L"������ �ּҸ� �Է��� �ּ��� : ";
	std::string ip_address;
	std::cin >> ip_address;

	Connect(ip_address.data());
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
