#include "pch.h"
#include "ServerCore.h"



ServerCore::ServerCore()
	: _iocp_core()
	, _is_running(false)
	, _id_counter(0)
	, _listen_socket(INVALID_SOCKET)
	, _accept_socket(INVALID_SOCKET)
	, _accept_overlapped(IoOperation::IO_ACCEPT)	
{
	Init();
}

ServerCore::~ServerCore()
{
	Release();
}

void ServerCore::Init()
{
	NetworkInit();
	std::cout << "Network Init Success\n";

	ThreadPoolInit();
	std::cout << "Thread Pool Init Success\n";

	Run();
}

void ServerCore::Run()
{
	BindAndListen();
	std::cout << "Listenning...\n";

	Accept();
}

void ServerCore::Release()
{
	for (auto& thread : _threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	_threads.clear();

	if (_listen_socket != INVALID_SOCKET) {
		closesocket(_listen_socket);
		_listen_socket = INVALID_SOCKET;
	}

	if (_accept_socket != INVALID_SOCKET) {
		closesocket(_accept_socket);
		_accept_socket = INVALID_SOCKET;
	}

	WSACleanup();
	std::cout << "Network Release Success\n";
}

void ServerCore::Accept()
{
	CreateAcceptSocket();
	BOOL ret = AcceptEx(_listen_socket, _accept_socket, _accept_overlapped.GetBuffer(), 0, 
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, _accept_overlapped.GetOverlapped());
	if (ret == FALSE) {
		// TEMP : ���� �޽��� ��� (�ӽ�)
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			std::cout << "AcceptEx Error : " << error << "\n";
		}
	}
}

void ServerCore::ProcessPacket(int64 id, BYTE* packet)
{
	// TODO : ��Ŷ �������ݿ� �°� �м� �� ó���ϱ�


}

void ServerCore::NetworkInit()
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
		std::cout << "WSAStartup Error\n";
		exit(-1);
	}

	CreateListenSocket();
	_iocp_core.AddSocket(_listen_socket, -1);
}

void ServerCore::BindAndListen()
{
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9000);		// TODO : ��Ʈ ��ȣ ���� (Core�� ����� ����)
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_listen_socket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
		std::cout << "bind Error\n";
		exit(-1);
	}

	if (listen(_listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "listen Error\n";
		exit(-1);
	}
}

void ServerCore::ThreadPoolInit()
{
	uint32 thread_count = std::thread::hardware_concurrency();
	_threads.reserve(thread_count);

	for (int i = 0; i < thread_count; ++i) {
		// TODO : �۾��ؾ� �ϴ� �Լ��� �°� thread ����
		//threads.emplace_back(worker, ...)
	}
}

void ServerCore::CreateListenSocket()
{
	_listen_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_listen_socket == INVALID_SOCKET) {
		std::cout << "listen socket Instance Error\n";
		exit(-1);
	}
}

void ServerCore::CreateAcceptSocket()
{
	_accept_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_accept_socket == INVALID_SOCKET) {
		std::cout << "accept socket Instance Error\n";
		exit(-1);
	}
}
