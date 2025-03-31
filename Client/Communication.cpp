#include "pch.h"
#include "Communication.h"



sf::SocketHandle MyTcpSocket::getHandlePublic()
{
	return getHandle();
}


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
	if (status == sf::Socket::Done) {
		std::wcout << L"서버와 연결되었습니다\n";
	}
	else if (status == sf::Socket::NotReady) {
		// 논블로킹 소켓에서는 NotReady가 발생 -> select()로 연결 확인
		fd_set writeSet;
		FD_ZERO(&writeSet);
		FD_SET(socket.getHandlePublic(), &writeSet);

		timeval timeout;
		timeout.tv_sec = 5;  // 최대 5초 동안 대기
		timeout.tv_usec = 0;

		int result = select(socket.getHandlePublic() + 1, nullptr, &writeSet, nullptr, &timeout);
		if (result > 0) {
			std::wcout << L"서버와 연결되었습니다.\n";
		}
		else {
			std::wcout << L"서버와 연결할 수 없습니다. 프로그램을 종료 후 다시 시도해 주세요.\n";
			exit(-1);
		}
	}
	else {
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

std::vector<char> Communication::Recv()
{
	char buffer[1024];
	std::size_t received = 0;
	sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
	if (status == sf::Socket::Done) {
		remain_buffer.insert(remain_buffer.end(), buffer, buffer + received);
	}
	else if (status == sf::Socket::NotReady) {
		// 데이터를 받은게 없음
		return std::vector<char>();
	}
	else if (status == sf::Socket::Disconnected) {
		std::wcout << L"서버와의 연결이 끊어졌습니다.\n";
		exit(-1);
	}
	else {
		std::wcout << L"Recv 오류!!!\n";
		std::wcout << L"에러 코드: " << WSAGetLastError() << std::endl;
		getchar();
		exit(-1);
	}
	
	remain_buffer.insert(remain_buffer.end(), buffer, buffer + received);

	if (remain_buffer.size() < HEADER_SIZE) {
		return std::vector<char>();
	}

	myNP::BASE_PACKET* packet = reinterpret_cast<myNP::BASE_PACKET*>(remain_buffer.data());
	if (remain_buffer.size() < packet->_size) {
		return std::vector<char>();
	}

	std::vector<char> data(remain_buffer.begin(), remain_buffer.begin() + packet->_size);
	remain_buffer.erase(remain_buffer.begin(), remain_buffer.begin() + packet->_size);

	return data;
}
