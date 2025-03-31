#include "pch.h"
#include "Communication.h"


// 기본 생성자
Communication::Communication()
{
	Init();
}

// 소멸자로 정리 필요한 항목 정리
Communication::~Communication()
{

}

// 초기에 필요한 작업들 ex) 논블로킹 설정
void Communication::Init()
{
	//socket.setBlocking(false);	// 소켓 통신을 논블로킹으로 설정
}

// 주소와 통신 연결
void Communication::Connect(const char* ip_address)
{
	sf::Socket::Status status = socket.connect(ip_address, PORT_NUM);
	if (status != sf::Socket::Done) {
		std::wcout << L"서버와 연결 할 수 없습니다. 프로그램을 종료 후 다시 시도해 주세요\n";
		getchar();
		exit(-1);
	}
}

void Communication::Send(char c)
{
	myNP::CS_KEY_INPUT* packet = new myNP::CS_KEY_INPUT{ static_cast<uint8_t>(game.GetID()), static_cast<uint8_t>(c) };
	sf::Socket::Status status = socket.send(packet, sizeof(myNP::CS_KEY_INPUT));
	if (status != sf::Socket::Done) {
		std::wcout << L"Send 오류!!!\n";
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
		std::wcout << L"Recv 오류!!!\n";
		getchar();
		exit(-1);
	}

	if (received != 2) {
		std::wcout << L"Recv byte 오류!!!\n";
		getchar();
		exit(-1);
	}

	std::string data = "";
	data += buffer[0];
	data += buffer[1];

	std::cout << "Recv x: " << static_cast<int>(data[0]) << ", y :" << static_cast<int>(data[1]) << '\n';

	return data;
}
