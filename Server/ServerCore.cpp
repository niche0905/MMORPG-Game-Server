#include "pch.h"
#include "ServerCore.h"
#include "IdleState.h"
#include "DeadState.h"
#include "ReturnState.h"
#include "AttackState.h"
#include "ChaseState.h"
#include "PM_IdleState.h"
#include "PM_RunState.h"
#include "PeaceMonster.h"
#include "FM_IdleState.h"
#include "FixedMonster.h"
#include "AgroMonster.h"
#include "NeutralMonster.h"



ServerCore::ServerCore()
	: _iocp_core()
	, _db_manager()
	, _sector_manager()
	, _is_running(false)
	, _id_counter(NUM_MONSTER)
	, _listen_socket(INVALID_SOCKET)
	, _accept_socket(INVALID_SOCKET)
	, _accept_overlapped(OverOperation::IO_ACCEPT)	
{
	Init();
}

ServerCore::~ServerCore()
{
	Release();
}

void ServerCore::Init()
{
	std::cout << "Is Developer?: ";
	int temp;
	std::cin >> temp;
	if (temp == 1)
		_is_developer = true;

	LocaleInit();

	WorldInit();

	NetworkInit();

	NPCInit();

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
		int32 error = WSAGetLastError();
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

void ServerCore::WorldInit()
{
	_world_map.LoadWorld();
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
	_threads.emplace_back([this]() {TimerWorker(); });

	std::cout << "Thread Pool Init Success\n";
	std::cout << "Thread Count : " << _threads.size() << "\n";
}

void ServerCore::StateInit()
{
	// TODO: 사용할 모든 State를 한번 Instance() 호출해 주어야 한다
	IdleState::Instance();
	DeadState::Instance();
	ReturnState::Instance();
	AttackState::Instance();
	ChaseState::Instance();
	PM_IdleState::Instance();
	PM_RunState::Instance();
	FM_IdleState::Instance();

	std::cout << "NPC States Init Success\n";
}

void ServerCore::NPCInit()
{
	StateInit();

	uint64 quater = NUM_MONSTER / 4;
	uint64 now_range = quater;
	uint64 i = 0;
	for ( ; i < now_range; ++i) {
		Creature* creature = new PeaceMonster{ i };
		std::string name = ("NPC" + std::to_string(i));
		creature->SetName(name);
		Position spawn_pos;
		while (true) {
			spawn_pos = { rand() % MAX_WIDTH, rand() % MAX_HEIGHT };
			if (not IsBlock(spawn_pos))
				break;
		}
		Bot* npc = static_cast<Bot*>(creature);
		npc->SetBasePosition(spawn_pos);
		creature->SetState(ST_INGAME);

		RegisterSector(i, creature->GetPosition());
		_clients.insert(std::make_pair(i, creature));
	}

	now_range += quater;
	for (; i < now_range; ++i) {
		Creature* creature = new FixedMonster{ i };
		std::string name = ("NPC" + std::to_string(i));
		creature->SetName(name);
		Position spawn_pos;
		while (true) {
			spawn_pos = { rand() % MAX_WIDTH, rand() % MAX_HEIGHT };
			if (not IsBlock(spawn_pos))
				break;
		}
		Bot* npc = static_cast<Bot*>(creature);
		npc->SetBasePosition(spawn_pos);
		creature->SetState(ST_INGAME);

		RegisterSector(i, creature->GetPosition());
		_clients.insert(std::make_pair(i, creature));
	}

	now_range += quater;
	for (; i < now_range; ++i) {
		Creature* creature = new NeutralMonster{ i };
		std::string name = ("NPC" + std::to_string(i));
		creature->SetName(name);
		Position spawn_pos;
		while (true) {
			spawn_pos = { rand() % MAX_WIDTH, rand() % MAX_HEIGHT };
			if (not IsBlock(spawn_pos))
				break;
		}
		Bot* npc = static_cast<Bot*>(creature);
		npc->SetBasePosition(spawn_pos);
		creature->SetState(ST_INGAME);

		RegisterSector(i, creature->GetPosition());
		_clients.insert(std::make_pair(i, creature));
	}

	now_range += quater;
	for (; i < now_range; ++i) {
		Creature* creature = new AgroMonster{ i };
		std::string name = ("NPC" + std::to_string(i));
		creature->SetName(name);
		Position spawn_pos;
		while (true) {
			spawn_pos = { rand() % MAX_WIDTH, rand() % MAX_HEIGHT };
			if (not IsBlock(spawn_pos))
				break;
		}
		Bot* npc = static_cast<Bot*>(creature);
		npc->SetBasePosition(spawn_pos);
		creature->SetState(ST_INGAME);

		RegisterSector(i, creature->GetPosition());
		_clients.insert(std::make_pair(i, creature));
	}

	std::cout << "NPC " << NUM_MONSTER << " Init Success\n";
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

		_ebr_sessions.StartEpoch();

		if (overlapped == nullptr) {
			std::cout << "overlapped is nullptr\n";

			_ebr_sessions.EndEpoch();
			continue;
		}

		ExOver* exp_overlapped = reinterpret_cast<ExOver*>(overlapped);
		OverOperation operation = exp_overlapped->GetOperation();
		if ((success == false) or ((operation == OverOperation::IO_RECV) or (operation == OverOperation::IO_SEND)) and (bytes_transferred == 0)) {
			if (_clients.count(key) != 0) {
				Creature* client = _clients.at(key);

				if (client) {
					Session* session = static_cast<Session*>(client);

					session->Disconnect();
				}
			}
			
			_ebr_sessions.EndEpoch();
			continue;
		}

		switch (exp_overlapped->GetOperation())
		{
		case OverOperation::IO_ACCEPT:
		{
			uint64 now_id = _id_counter++;
			_iocp_core.AddSocket(_accept_socket, now_id);
			Session* new_session = _ebr_sessions.Get(_accept_socket, now_id);
			_clients.insert(std::make_pair(now_id, new_session));
			new_session->Recv();

			Accept();
		}
		break;
		case OverOperation::IO_RECV:
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
		case OverOperation::IO_SEND:
		{
			delete exp_overlapped;
		}
		break;
		case OverOperation::DO_UPDATE:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsPlayer()) break;

			auto npc = static_cast<Bot*>(client);
			npc->Update();

			delete exp_overlapped;
		}
		break;
		case OverOperation::DO_REVIVE:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsPlayer()) break;

			auto npc = static_cast<Bot*>(client);
			npc->DoRevive();

			delete exp_overlapped;
		}
		break;
		case OverOperation::DB_LOGIN:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsNPC()) break;

			auto session = static_cast<Session*>(client);
			session->LoginDone();

			delete exp_overlapped;
		}
		break;
		case OverOperation::DB_LOGIN_FAIL:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsNPC()) break;

			auto session = static_cast<Session*>(client);
			session->LoginFalse();

			delete exp_overlapped;
		}
		break;
		case OverOperation::DB_REGISTER_FAIL:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsNPC()) break;

			auto session = static_cast<Session*>(client);
			session->RegisterFalse();

			delete exp_overlapped;
		}
		break;
		case OverOperation::DB_LOGOUT:
		{
			Creature* client = _clients.at(key);

			if (client == nullptr) break;

			if (client->IsNPC()) break;

			auto session = static_cast<Session*>(client);
			_login_user.at(session->GetUserID()) = INVALID_ID;
			_ebr_sessions.Reuse(session);
			_clients.at(key) = nullptr;

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

		_ebr_sessions.EndEpoch();

	}
}

void ServerCore::TimerWorker()
{
	using namespace std::chrono;

	while (true) {
		
		Event evt;
		auto current_time = system_clock::now();

		if (_timer_queue.try_pop(evt)) {
			if (evt._reserved_time > current_time) {
				_timer_queue.push(evt);
				std::this_thread::sleep_for(1ms);
				continue;
			}

			switch (evt._event_type)
			{
			case Event::EventType::EV_UPDATE:
			{
				ExOver* new_task = new ExOver(DO_UPDATE);
				_iocp_core.AddTask(evt._id, new_task);
			}
			break;
			
			case Event::EventType::EV_REVIVE:
			{
				ExOver* new_task = new ExOver(DO_REVIVE);
				_iocp_core.AddTask(evt._id, new_task);
			}
			break;

			default:
				break;
			}

		}
		else {
			std::this_thread::sleep_for(1ms);
		}
	}
}

const ServerCore::container<uint64, ServerCore::Client>& ServerCore::GetClients() const
{
	return _clients;
}

ServerCore::container<uint64, ServerCore::Client>& ServerCore::GetClients()
{
	return _clients;
}

const ServerCore::container<uint64, uint64>& ServerCore::GetMappingTable() const
{
	return _login_user;
}

ServerCore::container<uint64, uint64>& ServerCore::GetMappingTable()
{
	return _login_user;
}

void ServerCore::RegisterSector(uint64 id, int16 x, int16 y)
{
	_sector_manager.AddClient(id, x, y);
}

void ServerCore::RegisterSector(uint64 id, Position pos)
{
	_sector_manager.AddClient(id, pos.x, pos.y);
}

void ServerCore::MoveSector(uint64 id, int16 old_x, int16 old_y, int16 new_x, int16 new_y)
{
	_sector_manager.MoveClient(id, old_x, old_y, new_x, new_y);
}

void ServerCore::MoveSector(uint64 id, Position old_pos, Position new_pos)
{
	_sector_manager.MoveClient(id, old_pos.x, old_pos.y, new_pos.x, new_pos.y);
}

void ServerCore::RemoveSector(uint64 id, int16 x, int16 y)
{
	_sector_manager.RemoveClient(id, x, y);
}

void ServerCore::RemoveSector(uint64 id, Position pos)
{
	_sector_manager.RemoveClient(id, pos.x, pos.y);
}

std::unordered_set<uint64> ServerCore::GetClientList(short x, short y)
{
	std::unordered_set<uint64> closed_clients;
	_sector_manager.GetClientList(x, y, closed_clients);
	return closed_clients;
}

std::unordered_set<uint64> ServerCore::GetClientList(Position pos)
{
	std::unordered_set<uint64> closed_clients;
	_sector_manager.GetClientList(pos.x, pos.y, closed_clients);
	return closed_clients;
}

void ServerCore::AddTask(uint64 id, ExOver* new_task)
{
	_iocp_core.AddTask(id, new_task);
}

void ServerCore::AddTimerEvent(const Event& timer_event)
{
	_timer_queue.push(timer_event);
}

void ServerCore::AddRequestDB(const DatabaseEvent& db_event)
{
	_db_manager.AddEventDB(db_event);
}

bool ServerCore::IsBlock(int16 x, int16 y) const
{
	return _world_map.IsBlock(x, y);
}

bool ServerCore::IsBlock(const Position& pos) const
{
	return _world_map.IsBlock(pos);
}

bool ServerCore::IsDev() const
{
	return _is_developer;
}
