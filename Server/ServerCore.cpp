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
	_is_running = false;
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
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			PrintErrorMessage(L"AcceptEx", error);
		}
	}
}

void ServerCore::ProcessPacket(int64 id, BYTE* packet)
{
	// TODO : 패킷 프로토콜에 맞게 분석 후 처리하기


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
	server_addr.sin_port = htons(9000);		// TODO : 포트 번호 설정 (Core의 상수로 설정)
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

	_is_running = true;
	for (int i = 0; i < thread_count; ++i) {
		// TODO : 작업해야 하는 함수에 맞게 thread 생성
		//threads.emplace_back(worker, ...)
		_threads.emplace_back([this]() { Worker(); });
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

void ServerCore::Worker()
{
	while (_is_running) {

		DWORD bytes_transferred = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED overlapped = nullptr;

		bool success = _iocp_core.Dispatch(bytes_transferred, key, overlapped);

		if (overlapped == nullptr) {
			std::cout << "overlapped is nullptr\n";
			continue;
		}

		ExOver* exp_overlapped = reinterpret_cast<ExOver*>(overlapped);
		IoOperation operation = exp_overlapped->GetOperation();
		if ((success == false) or ((operation == IoOperation::IO_RECV) or (operation == IoOperation::IO_SEND)) and (bytes_transferred == 0)) {
			if (_clients.count(key) != 0) {
				_clients.at(key).store(nullptr);	// <- _clients.at(key) = nullptr;
			}
			
			continue;
		}

		switch (exp_overlapped->GetOperation())
		{
		case IoOperation::IO_ACCEPT:
		{
			int now_id = _id_counter++;
			_iocp_core.AddSocket(_accept_socket, now_id);
			LocalClient new_session = std::make_shared<Session>(_accept_socket, now_id);
			_clients.insert(std::make_pair(now_id, new_session));
			new_session->Recv();

			Accept();
		}
		break;
		case IoOperation::IO_RECV:
		{
			LocalClient session = _clients.at(key).load();
			if (session == nullptr) {
				std::cout << "Session is nullptr\n";
				break;
			}

			session->ReassemblePacket(bytes_transferred);

			session->Recv();
		}
		break;
		case IoOperation::IO_SEND:
		{
			delete exp_overlapped;
		}
		break;
		default:
		{
			std::cout << "Unknown IO Operation\n";
			exit(-1);	// 정의되지 않은 Operation이 발생했을 경우 종료하는 것이 맞음
		}
		break;
		} 
	}
}
