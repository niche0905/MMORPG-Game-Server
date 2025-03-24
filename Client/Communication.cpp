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
	socket.setBlocking(false);	// 소켓 통신을 논블로킹으로 설정
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
