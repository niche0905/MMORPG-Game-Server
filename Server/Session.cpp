#include "pch.h"
#include "Session.h"


Session::Session()
{
	// unordered_map을 사용할 것이기에 기본 생성자가 호출되면 안되는 것임
	std::cout << "Session 기본 생성자 호출 에러\n";
	exit(-1);
}

Session::Session(SOCKET socket, uint64 id) 
	: Creature{ id, true }
	, _socket(socket)
	, _remain_size(0)
	, _recv_overlapped(OverOperation::IO_RECV)
{
	
}

Session::~Session()
{
	Disconnect();

	// TODO : 소켓을 닫아도 된다면 판단 후에 닫기 or IOCP 소켓 작업 다 캔슬하고 바로 닫기
	closesocket(_socket);
}

void Session::Disconnect()
{
	if (_state != ST_CLOSE) {

		_view_lock.lock();
		std::unordered_set<uint64> view_list = _view_list;
		_view_lock.unlock();

		for (uint64 client_id : view_list) {

			if (client_id == _id or ::IsNPC(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == State::ST_ALLOC or state == State::ST_CLOSE) continue;

			Session* session = static_cast<Session*>(client);
			session->SendLeaveCreature(_id);
		}
		server.RemoveSector(_id, _position);
	}

	_state = ST_CLOSE;
}

void Session::Reset(SOCKET socket, uint64 id)
{
	_id = id;
	_socket = socket;
	_remain_size = 0;
	_recv_overlapped.Reset(0);
}

bool Session::IsPlayer() const
{
	return true;
}

bool Session::IsNPC() const
{
	return false;
}

void Session::Send(void* packet)
{
	ExOver* send_overlapped = new ExOver(OverOperation::IO_SEND);
	send_overlapped->SetBuffer(packet, static_cast<int>(reinterpret_cast<BYTE*>(packet)[0/*패킷 사이즈가 저장된 위치*/]));

	DWORD sent_bytes = 0;
	auto ret = WSASend(_socket, send_overlapped->GetWSABuf(), 1, &sent_bytes, 0, send_overlapped->GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			PrintErrorMessage(L"WSASend", error);
		}
	}
}

void Session::Recv()
{
	_recv_overlapped.Reset(_remain_size);

	DWORD flags = 0;
	auto ret = WSARecv(_socket, _recv_overlapped.GetWSABuf(), 1, nullptr, &flags, _recv_overlapped.GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			PrintErrorMessage(L"WSARecv", error);
		}
	}
}

void Session::ProcessPacket(BYTE* packet)
{
	BASE_PACKET* packet_header = reinterpret_cast<BASE_PACKET*>(packet);
	uint8 packet_id = packet_header->_packet_id;

	switch (packet_id)
	{
	case PacketID::C2S_LOGIN:
	{
		LoginProcess(packet);
	}
	break;

	case PacketID::C2S_MOVE:
	{
		MoveProcess(packet);
	}
	break;

	case PacketID::C2S_CHAT:
	{
		ChatProcess(packet);
	}
	break;

	}
}

void Session::ReassemblePacket(DWORD recv_size)
{
	BYTE* packet = _recv_overlapped.GetBuffer();

	int data_size = _remain_size + recv_size;
	int proccess_size = 0;
	while (packet < _recv_overlapped.GetBuffer() + data_size) {
		BYTE packet_size = packet[0];

		if (data_size >= packet_size) {
			ProcessPacket(packet);
			packet += packet_size;
			proccess_size += packet_size;
		}
		else {
			break;
		}
	}

	if (data_size - proccess_size > 0) {
		_recv_overlapped.CopyToBuffer(packet, data_size - proccess_size);
	}
	_remain_size = data_size - proccess_size;
}

void Session::LoginProcess(BYTE* packet)
{
	CS_LOGIN_PACKET* login_packet = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
	SetName(login_packet->_name);

	_position.x = rand() % MAX_WIDTH;
	_position.y = rand() % MAX_WIDTH;

	server.RegisterSector(_id, _position);

	_state = State::ST_INGAME;

	// TODO: 값 제대로 사용하기
	SC_LOGIN_ALLOW_PACKET login_allow_packet{ _id, _position.x, _position.y, 100, 100, 1, 0 };
	Send(&login_allow_packet);

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, 0 };

	std::unordered_set<uint64> user_list = server.GetClientList(_position);
	for (uint64 client_id : user_list) {
		if (client_id == _id) continue;

		auto client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == State::ST_ALLOC or state == State::ST_CLOSE) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Position client_pos = client->GetPosition();
		SC_ENTER_PACKET other_enter_packet{ client_id, client_pos.x, client_pos.y, client->GetName().data(), 0, 1 };
		SendNewCreature(client_id, &other_enter_packet);

		if (client->IsPlayer()) {
			auto session = static_cast<Session*>(client);
			session->ProcessCloseCreature(_id, &enter_packet, nullptr);
		}
		else if (client->IsNPC()) {
			auto npc = static_cast<Bot*>(client);
			npc->WakeUp();
		}
	}
}

void Session::MoveProcess(BYTE* packet)
{
	CS_MOVE_PACKET* move_packet = reinterpret_cast<CS_MOVE_PACKET*>(packet);
	_last_move_time = move_packet->_move_time;

	Position new_position = _position;
	switch (move_packet->_direction)
	{
	case MOVE_UP:
		if (new_position.y > 0) --new_position.y;
		break;
	case MOVE_DOWN:
		if (new_position.y < MAX_HEIGHT - 1) ++new_position.y;
		break;
	case MOVE_LEFT:
		if (new_position.x > 0) --new_position.x;
		break;
	case MOVE_RIGHT:
		if (new_position.x < MAX_WIDTH - 1) ++new_position.x;
		break;
	}

	if (_position == new_position) {
		// 변한게 없다면 스스로에게만 시간 Update
		SelfUpdate();
		return;
	}
	server.MoveSector(_id, _position, new_position);

	_position = new_position;
	SelfUpdate();

	std::unordered_set<uint64> near_list;
	_view_lock.lock();
	std::unordered_set<uint64> old_list = _view_list;
	_view_lock.unlock();

	std::unordered_set<uint64> closed_clients = server.GetClientList(_position.x, _position.y);
	for (uint64 client_id : closed_clients) {

		if (client_id == _id) continue;	// 내 ID라면 무시

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		uint8 state = client->GetState();
		if (state == State::ST_ALLOC or state == State::ST_CLOSE) continue;	// 게임 참여 중 아니라면 무시

		// view list 판단
		if (client->CanSee(_position, VIEW_RANGE)) {	// 보이는 경우
			near_list.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, 0 };		// TODO: 값 제대로
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y, _last_move_time };

	for (uint64 client_id : near_list) {

		if (client_id == _id) continue;	// 내 ID라면 무시 위에서 거르지만 혹시 모르니깐

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		if (client->IsPlayer()) {
			auto session = static_cast<Session*>(client);
			session->ProcessCloseCreature(_id, &enter_packet, &update_packet);
		}
		else {
			auto npc = static_cast<Bot*>(client);
			npc->WakeUp();
		}

		if (old_list.count(client_id) == 0) {
			Position pos = client->GetPosition();
			SC_ENTER_PACKET object_enter_packet{ client_id, pos.x, pos.y, client->GetName().data(), 0, 0 };		// TODO: 값 제대로

			SendNewCreature(client_id, &object_enter_packet);
		}
	}

	// view_list 벗어났다면 leave_packet 전송
	for (uint64 client_id : old_list) {

		if (client_id == _id) continue;	// 내 ID라면 무시 위에서 거르지만 혹시 모르니깐

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		if (client->IsNPC()) continue;

		if (near_list.count(client_id) == 0) {
			auto session = static_cast<Session*>(client);
			SendLeaveCreature(client_id);
			session->SendLeaveCreature(_id);
		}
	}
}

void Session::ChatProcess(BYTE* packet)
{
	// TODO: 내용 채우기 (일정 범위 내 전송)
}

void Session::ProcessCloseCreature(uint64 id, void* enter_packet, void* move_packet)
{
	_view_lock.lock();
	if (_view_list.count(id) == 0) {	// 기존에 없던 생명체
		_view_list.insert(id);
		_view_lock.unlock();

		Send(enter_packet);
	}
	else {								// 기존에 있던 생명체
		_view_lock.unlock();

		Send(move_packet);
	}
}

void Session::SendNewCreature(uint64 id, void* enter_packet)
{
	_view_lock.lock();
	if (_view_list.count(id) != 0) {
		_view_lock.unlock();

		return;
	}

	_view_list.insert(id);
	_view_lock.unlock();
	Send(enter_packet);
}

void Session::SendLeaveCreature(uint64 id)
{
	_view_lock.lock();
	if (_view_list.count(id) == 0) {
		_view_lock.unlock();
		return;
	}
	_view_list.erase(id);
	_view_lock.unlock();

	SC_LEAVE_PACKET leave_packet{ id };
	Send(&leave_packet);
}

void Session::SelfUpdate()
{
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y, _last_move_time };

	Send(&update_packet);
}
