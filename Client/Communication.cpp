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
		std::wcout << L"서버와 바로 연결되었습니다\n";
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

void Communication::Send(BYTE* buffer, uint16 len)
{
	std::size_t total_sent = 0;
	std::size_t size = len;

	while (total_sent < size) {
		std::size_t sent;
		sf::Socket::Status status = socket.send(buffer + total_sent, size - total_sent, sent);

		if (status == sf::Socket::Done || status == sf::Socket::Partial) {
			total_sent += sent;
		}
		else {
			std::wcout << L"Send 오류 발생!\n";
			exit(-1);
		}
	}
}

std::vector<BYTE> Communication::Recv()
{
	char buffer[1024];
	std::size_t received = 0;
	sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
	if (status == sf::Socket::Done) {
		remain_buffer.insert(remain_buffer.end(), buffer, buffer + received);
	}
	else if (status == sf::Socket::NotReady) {
		// 데이터를 받은게 없음
		return std::vector<BYTE>();
	}
	else if (status == sf::Socket::Disconnected) {
		std::wcout << L"Recv 중 서버와의 연결이 끊어졌습니다.\n";
		exit(-1);
	}
	else {
		std::wcout << L"Recv 오류!!!\n";
		std::wcout << L"에러 코드: " << WSAGetLastError() << std::endl;
		getchar();
		exit(-1);
	}

	size_t buffer_data_size = remain_buffer.size();
	size_t proccess_size = 0;
	while (buffer_data_size >= HEADER_SIZE) {

		BASE_PACKET* header = reinterpret_cast<BASE_PACKET*>(remain_buffer.data() + proccess_size);
		if (buffer_data_size < header->_size) break;

		buffer_data_size -= header->_size;
		proccess_size += header->_size;
	}
	
	std::vector<BYTE> data(remain_buffer.begin(), remain_buffer.begin() + proccess_size);
	remain_buffer.erase(remain_buffer.begin(), remain_buffer.begin() + proccess_size);

	return data;
}
