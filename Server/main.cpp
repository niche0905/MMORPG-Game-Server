#include <iostream>
#include <random>
#include <WS2tcpip.h>
#pragma comment (lib, "WS2_32.LIB")

constexpr int PORT_NUM{ /*A*/21004 };
constexpr int BUF_SIZE{ 200 };

char pos_x, pos_y;

SOCKET server_socket;
SOCKET client_socket;
char recv_buffer[BUF_SIZE];
WSABUF recv_wsabuf[1];
WSAOVERLAPPED recv_over;

std::default_random_engine dre;
std::uniform_int_distribution<int> uid{ 0, 7 };

void print_error_message(int err_no);
void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag);
void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag);

void CALLBACK send_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag)
{
	std::cout << "Send 성공\n";
	
	recv_wsabuf[0].buf = recv_buffer;
	recv_wsabuf[0].len = sizeof(recv_buffer);

	DWORD recv_flag = 0;
	ZeroMemory(&recv_over, sizeof(recv_over));
	WSARecv(client_socket, recv_wsabuf, 1, NULL, &recv_flag, &recv_over, recv_callback);
}

void CALLBACK recv_callback(DWORD err, DWORD num_bytes, LPWSAOVERLAPPED p_over, DWORD flag)
{
	for (int i = 0; i < num_bytes; ++i) {
		char dir = recv_buffer[i];

		switch (dir)
		{
		case 0:
			if (pos_y - 1 >= 0)
				pos_y -= 1;
			break;
		case 1:
			if (pos_x - 1 >= 0)
				pos_x -= 1;
			break;
		case 2:
			if (pos_y + 1 < 8)
				pos_y += 1;
			break;
		case 3:
			if (pos_x + 1 < 8)
				pos_x += 1;
			break;
		default:
			std::cout << "에러, 의도되지 않은 방향 " << dir << '\n';
			break;
		}
	}

	std::cout << "Recv 성공\n";

	char buffer[2];
	buffer[0] = pos_x;
	buffer[1] = pos_y;

	WSABUF wsabuf[1];
	wsabuf[0].buf = buffer;
	wsabuf[0].len = 2;

	DWORD sent_size;
	WSAOVERLAPPED send_over;
	ZeroMemory(&send_over, sizeof(send_over));

	int ret = WSASend(client_socket, wsabuf, 1, &sent_size, 0, &send_over, send_callback);
	if (SOCKET_ERROR == ret) {
		auto err_code = WSAGetLastError();
		print_error_message(err_code);
	}
}


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


int main()
{
	std::wcout.imbue(std::locale("korean"));

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);	// MS 네트워크를 사용하지 않고 표준 네트워크를 사용하기 위한 호출

	server_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
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

	INT addr_size = sizeof(SOCKADDR_IN);
	SOCKET client_socket = WSAAccept(server_socket, reinterpret_cast<sockaddr*>(&addr), &addr_size, NULL, NULL);

	std::cout << "클라이언트 접속\n";

	pos_x = uid(dre); pos_y = uid(dre);
	char buffer[2];
	buffer[0] = pos_x;
	buffer[1] = pos_y;

	WSABUF wsabuf[1];
	wsabuf[0].buf = buffer;
	wsabuf[0].len = 2;

	DWORD sent_size;
	WSAOVERLAPPED send_over;
	ZeroMemory(&send_over, sizeof(send_over));

	int ret = WSASend(client_socket, wsabuf, 1, &sent_size, 0, &send_over, send_callback);
	if (SOCKET_ERROR == ret) {
		auto err_code = WSAGetLastError();
		print_error_message(err_code);
	}

	while (true)
	{
		SleepEx(0, TRUE);
	}

	closesocket(client_socket);
	WSACleanup();

	return 0;
}