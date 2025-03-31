#include "pch.h"
#include "Communication.h"



sf::SocketHandle MyTcpSocket::getHandlePublic()
{
	return getHandle();
}


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
	socket.setBlocking(false);	// ���� ����� ����ŷ���� ����
}

// �ּҿ� ��� ����
void Communication::Connect(const char* ip_address)
{
	sf::Socket::Status status = socket.connect(ip_address, PORT_NUM);
	if (status == sf::Socket::Done) {
		std::wcout << L"������ �ٷ� ����Ǿ����ϴ�\n";
	}
	else if (status == sf::Socket::NotReady) {
		// ����ŷ ���Ͽ����� NotReady�� �߻� -> select()�� ���� Ȯ��
		fd_set writeSet;
		FD_ZERO(&writeSet);
		FD_SET(socket.getHandlePublic(), &writeSet);

		timeval timeout;
		timeout.tv_sec = 5;  // �ִ� 5�� ���� ���
		timeout.tv_usec = 0;

		int result = select(socket.getHandlePublic() + 1, nullptr, &writeSet, nullptr, &timeout);
		if (result > 0) {
			std::wcout << L"������ ����Ǿ����ϴ�.\n";
		}
		else {
			std::wcout << L"������ ������ �� �����ϴ�. ���α׷��� ���� �� �ٽ� �õ��� �ּ���.\n";
			exit(-1);
		}
	}
	else {
		std::wcout << L"������ ���� �� �� �����ϴ�. ���α׷��� ���� �� �ٽ� �õ��� �ּ���\n";
		getchar();
		exit(-1);
	}
}

void Communication::Send(char c)
{
	myNP::CS_KEY_INPUT* packet = new myNP::CS_KEY_INPUT{ static_cast<uint8_t>(game.GetID()), static_cast<uint8_t>(c) };
	char* buffer = reinterpret_cast<char*>(packet);

	std::size_t total_sent = 0;
	std::size_t size = sizeof(myNP::CS_KEY_INPUT);

	while (total_sent < size) {
		std::size_t sent;
		sf::Socket::Status status = socket.send(buffer + total_sent, size - total_sent, sent);

		if (status == sf::Socket::Done || status == sf::Socket::Partial) {
			total_sent += sent;
		}
		else {
			std::wcout << L"Send ���� �߻�!\n";
			exit(-1);
		}
	}

	std::cout << "Send dir: " << static_cast<int>(c) << '\n';
	delete packet;
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
		// �����͸� ������ ����
		return std::vector<char>();
	}
	else if (status == sf::Socket::Disconnected) {
		std::wcout << L"�������� ������ ���������ϴ�.\n";
		exit(-1);
	}
	else {
		std::wcout << L"Recv ����!!!\n";
		std::wcout << L"���� �ڵ�: " << WSAGetLastError() << std::endl;
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

	std::cout << static_cast<int>(data[0]) << ' ' << static_cast<int>(data[1]) << '\n';

	return data;
}
