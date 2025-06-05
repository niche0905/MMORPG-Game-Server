#include "pch.h"
#include "ServerCore.h"



ServerCore::ServerCore()
	: _iocp_core()
	, _db_manager()
	, _is_running(false)
	, _id_counter(NUM_MONSTER)
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
	LocaleInit();

	NetworkInit();

	DatabaseInit();

	ThreadPoolInit();
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
		thread.join();
	}
	_threads.clear();

	_is_running = false;
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

void ServerCore::LocaleInit()
{
	setlocale(LC_ALL, "korean");
	std::wcout.imbue(std::locale("korean"));
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

	std::cout << "Network Init Success\n";
}

void ServerCore::BindAndListen()
{
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_listen_socket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
		std::cout << "bind Error\n";
		exit(-1);
	}

	if (listen(_listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "listen Error\n";
		exit(-1);
	}

	std::cout << "Bind and Listen Success\n";
}

void ServerCore::DatabaseInit()
{
	_db_manager.Init();

	_db_manager.DatabaseThread();
}

void ServerCore::ThreadPoolInit()
{
	uint32 thread_count = std::thread::hardware_concurrency();
	_threads.reserve(thread_count);

	_is_running = true;
	for (int i = 0; i < thread_count; ++i) {
		_threads.emplace_back([this, i]() { SetThreadId(i); Worker(); });
	}

	std::cout << "Thread Pool Init Success\n";
	std::cout << "Thread Count : " << thread_count << "\n";
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

		_ebr_sessions.StartEpoch();

		DWORD bytes_transferred = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED overlapped = nullptr;

		bool success = _iocp_core.Dispatch(bytes_transferred, key, overlapped);


		if (overlapped == nullptr) {
			std::cout << "overlapped is nullptr\n";

			_ebr_sessions.EndEpoch();
			continue;
		}

		ExOver* exp_overlapped = reinterpret_cast<ExOver*>(overlapped);
		IoOperation operation = exp_overlapped->GetOperation();
		if ((success == false) or ((operation == IoOperation::IO_RECV) or (operation == IoOperation::IO_SEND)) and (bytes_transferred == 0)) {
			if (_clients.count(key) != 0) {
				Creature* client = _clients.at(key);

				if (client) {
					Session* session = static_cast<Session*>(client);

					_ebr_sessions.Reuse(session);
					_clients.at(key) = nullptr;
				}
			}
			
			_ebr_sessions.EndEpoch();
			continue;
		}

		switch (exp_overlapped->GetOperation())
		{
		case IoOperation::IO_ACCEPT:
		{
			int now_id = _id_counter++;
			_iocp_core.AddSocket(_accept_socket, now_id);
			Session* new_session = _ebr_sessions.Get(_accept_socket, now_id);
			_clients.insert(std::make_pair(now_id, new_session));
			new_session->Recv();

			Accept();
		}
		break;
		case IoOperation::IO_RECV:
		{
			Creature* client = _clients.at(key);
			if (client == nullptr) {
				std::cout << "Client is nullptr\n";
				break;
			}

			auto session = static_cast<Session*>(client);
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

	_ebr_sessions.EndEpoch();

}
