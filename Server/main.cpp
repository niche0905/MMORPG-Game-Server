#include "pch.h"
#include <iostream>
#include <random>
#include <unordered_map>
#include <WS2tcpip.h>
#include "Packet.h"

#pragma comment (lib, "WS2_32.LIB")

//constexpr int PORT_NUM{ /*A*/21004 };
//constexpr int BUF_SIZE{ 200 };

constexpr int MIN_X{ 0 };
constexpr int MIN_Y{ 0 };
constexpr int MAX_X{ 8 };
constexpr int MAX_Y{ 8 };

class EXP_OVER;
class SESSION;


std::unordered_map<int64_t, SESSION> g_clients;

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 0, 7 };

void print_error_message(int err_no);
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag);
void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag);


int main()
{
	std::wcout.imbue(std::locale("korean"));

	WSADATA WSAData;
	int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &WSAData);	// MS 네트워크를 사용하지 않고 표준 네트워크를 사용하기 위한 호출
	if (wsaStartupResult != 0) {
		std::wcout << L"WSAStartup 실패, 오류 코드: " << wsaStartupResult << std::endl;
		return -1;  // 프로그램 종료
	}

	SOCKET server_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (server_socket == INVALID_SOCKET) {
		auto err_code = WSAGetLastError();
		print_error_message(err_code);
	}
	else {
		std::cout << "Server Socket Created\n";
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(server_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(SOCKADDR_IN));

	listen(server_socket, SOMAXCONN);

	int64_t client_id{ 1 };

	while (true) {

		INT addr_size = sizeof(SOCKADDR_IN);
		SOCKET client_socket = WSAAccept(server_socket, reinterpret_cast<sockaddr*>(&addr), &addr_size, NULL, NULL);

		if (client_socket == INVALID_SOCKET) {
			int err_code = WSAGetLastError();
			print_error_message(err_code);
			continue;
		}
		std::wcout << L"클라이언트 연결 수락됨, id: " << client_id << std::endl;

		g_clients.try_emplace(client_id, client_id, client_socket);
		client_id++;
	}

	std::cout << "Server Close\n";

	closesocket(server_socket);
	WSACleanup();

	return 0;
}

class EXP_OVER
{
public:
	WSAOVERLAPPED _over;
	int64_t _id;
	char _buffer[1024];
	WSABUF _wsabuf[1];

public:
	EXP_OVER()
	{
		std::cout << "EXP_OVER 기본 생성자 호출;;;\n";
		exit(-1);
	}

	EXP_OVER(int64_t id, char* message, int size)
		: _id{ id }
	{
		ZeroMemory(&_over, sizeof(_over));

		if (message != nullptr)
			memcpy(_buffer, message, size);

		_wsabuf[0].buf = _buffer;
		_wsabuf[0].len = static_cast<ULONG>(size);
	}

	void Init()
	{
		ZeroMemory(&_over, sizeof(_over));
	}

};

class SESSION
{
private:
	SOCKET _socket;
	int64_t _id;

	EXP_OVER _recv_exp;

	int _x, _y;

public:
	SESSION()
	{
		std::cout << "SESSION 기본 생성자 호출;;;\n";
		exit(-1);
	}

	SESSION(int64_t id, SOCKET socket)
		: _id{ id }
		, _socket{ socket }
		, _recv_exp{_id, nullptr, 1024}
	{
		_x = uid(dre);
		_y = uid(dre);

		// 다른 클라이언트들에게 Login 통보
		myNP::SC_LOGIN_USER* packet1 = new myNP::SC_LOGIN_USER{ static_cast<uint8_t>(_id), static_cast<uint8_t>(_x), static_cast<uint8_t>(_y) };
		do_send(_id, reinterpret_cast<char*>(packet1), sizeof(myNP::SC_LOGIN_USER));

		for (auto& client : g_clients) {
			if (client.second.Get_ID() != _id) {
				client.second.do_send(client.second.Get_ID(), reinterpret_cast<char*>(packet1), sizeof(myNP::SC_LOGIN_USER));
			}
		}
		delete packet1;

		// 본인 클라이언트에게 Login 전달
		for (auto& client : g_clients) {
			if (client.second.Get_ID() != _id) {
				std::pair<int, int> pos = client.second.Get_Pos();
				myNP::SC_LOGIN_USER* packet2 = new myNP::SC_LOGIN_USER{ static_cast<uint8_t>(client.second.Get_ID()), static_cast<uint8_t>(pos.first), static_cast<uint8_t>(pos.second) };
				do_send(_id, reinterpret_cast<char*>(packet2), sizeof(myNP::SC_LOGIN_USER));
				delete packet2;
			}
		}

		do_recv();
	}

	~SESSION()
	{
		myNP::SC_LOGOUT_USER* packet = new myNP::SC_LOGOUT_USER{ static_cast<uint8_t>(_id) };

		for (auto& client : g_clients) {
			if (client.second.Get_ID() != _id) {
				client.second.do_send(_id, reinterpret_cast<char*>(packet), sizeof(myNP::SC_LOGOUT_USER));
			}
		}

		delete packet;

		std::cout << _id << " Close Socket\n";
		closesocket(_socket);
	}

	int64_t Get_ID() const { return _id; }
	std::pair<int, int> Get_Pos() const { return { _x, _y }; }

	void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag)
	{
		// recv 한 내용 처리
		// 키 입력에 따라 위치를 이동시키고
		// 해당 움직임을 브로드캐스트

		if (err != 0) {
			print_error_message(err);
			g_clients.erase(_id);
			return;
		}

		if (num_bytes == 0) {
			std::cout << _id << "더 이상 데이터가 오지 않음\n";
			g_clients.erase(_id);
			return;
		}

		EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
		myNP::BASE_PACKET* base_packet = reinterpret_cast<myNP::BASE_PACKET*>(exp_over->_buffer);
		switch (base_packet->_id)
		{
		case myNP::PacketID::CS_KEY_INPUT:
		{
			myNP::CS_KEY_INPUT* packet = reinterpret_cast<myNP::CS_KEY_INPUT*>(exp_over->_buffer);
			std::cout << _id << " 방향키를 받았음 " << static_cast<int>(packet->_direction) << '\n';

			switch (packet->_direction)
			{
			case 0:
				if (_y - 1 >= MIN_Y)
					_y -= 1;
				break;
			case 1:
				if (_x - 1 >= MIN_X)
					_x -= 1;
				break;
			case 2:
				if (_y + 1 < MAX_Y)
					_y += 1;
				break;
			case 3:
				if (_x + 1 < MAX_X)
					_x += 1;
				break;
			default:
				std::cout << "CS_KEY_INPUT 에러, 의도되지 않은 방향 " << packet->_direction << '\n';
				break;
			}
		}
			break;
		default:
			std::cout << "패킷 id 없는 패킷 오류\n";
			break;
		}

		myNP::SC_MOVE_USER* move_packet = new myNP::SC_MOVE_USER{ static_cast<uint8_t>(_id), static_cast<uint8_t>(_x), static_cast<uint8_t>(_y) };

		for (auto& client : g_clients) {
			client.second.do_send(client.first, reinterpret_cast<char*>(move_packet), sizeof(myNP::SC_MOVE_USER));
		}

		delete move_packet;

		do_recv();
	}

private:
	void do_send(int64_t id, char* message, int size)
	{
		EXP_OVER* send_exp = new EXP_OVER{ id, message, size };
		DWORD sent_size;
		auto ret = WSASend(_socket, send_exp->_wsabuf, 1, &sent_size, 0, &send_exp->_over, ::send_callback);
		if (SOCKET_ERROR == ret) {
			int err_no = WSAGetLastError();

			if (err_no == WSA_IO_PENDING) {
				return;
			}

			print_error_message(err_no);
			exit(-1);
		}
	}

	void do_recv()
	{
		_recv_exp.Init();

		DWORD recv_flag = 0;
		auto ret = WSARecv(_socket, _recv_exp._wsabuf, 1, NULL, &recv_flag, &_recv_exp._over, ::recv_callback);
		if (SOCKET_ERROR == ret) {
			auto err_no = WSAGetLastError();
			if (WSA_IO_PENDING != err_no) {
				print_error_message(err_no);
				//exit(-1);
				return;
			}
		}
	}

};

void print_error_message(int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::wcout << L" 에러 : " << lpMsgBuf << std::endl;
	while (true);
	// 디버깅 용
	LocalFree(lpMsgBuf);
}

void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag)
{
	EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
	delete exp_over;
}

void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag)
{
	EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
	int64_t my_id = exp_over->_id;
	g_clients[my_id].recv_callback(err, num_bytes, p_over, flag);
}
