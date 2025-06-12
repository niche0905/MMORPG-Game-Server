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
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

			Session* session = static_cast<Session*>(client);
			session->SendLeaveCreature(_id);
		}
		server.RemoveSector(_id, _position);
	}

	_state = ST_CLOSE;

	closesocket(_socket);
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

uint16 Session::GetMaxHP() const
{
	return _basic_stats.HP + _temp_stats.HP + _equip_stats.HP;
}

void Session::TakeDamage(uint16 damage)
{
	Creature::TakeDamage(damage);

	SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };

	_view_lock.lock();
	std::unordered_set<uint64> view_list = _view_list;
	_view_lock.unlock();

	for (uint64 client_id : view_list) {

		if (client_id == _id or ::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&hp_change_packet);
	}
}

void Session::Send(void* packet)
{
	ExOver* send_overlapped = new ExOver(OverOperation::IO_SEND);
	send_overlapped->SetBuffer(packet, static_cast<int>(reinterpret_cast<BYTE*>(packet)[0/*패킷 사이즈가 저장된 위치*/]));

	DWORD sent_bytes = 0;
	auto ret = WSASend(_socket, send_overlapped->GetWSABuf(), 1, &sent_bytes, 0, send_overlapped->GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		int32 error = WSAGetLastError();
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
		int32 error = WSAGetLastError();
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

	case PacketID::C2S_ATTACK:
	{
		AttackProcess(packet);
	}
	break;

	}
}

void Session::ReassemblePacket(DWORD recv_size)
{
	BYTE* packet = _recv_overlapped.GetBuffer();

	int32 data_size = _remain_size + recv_size;
	int32 proccess_size = 0;
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

	_state = GameState::ST_INGAME;

	// TODO: 값 제대로 사용하기
	SC_LOGIN_ALLOW_PACKET login_allow_packet{ _id, _position.x, _position.y, GetMaxHP(), _hp, 0, _level, _exp};
	Send(&login_allow_packet);

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, _level };
	SC_MOVE_PACKET move_packet{ _id, _position.x, _position.y };

	std::unordered_set<uint64> user_list = server.GetClientList(_position);
	for (uint64 client_id : user_list) {
		if (client_id == _id) continue;

		auto client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Position client_pos = client->GetPosition();
		SC_ENTER_PACKET other_enter_packet{ client_id, client_pos.x, client_pos.y, client->GetName().data(), 0, _level };
		SendNewCreature(client_id, &other_enter_packet);

		if (client->IsPlayer()) {
			auto session = static_cast<Session*>(client);
			session->ProcessCloseCreature(_id, &enter_packet, &move_packet);
		}
		else if (client->IsNPC()) {
			auto npc = static_cast<Bot*>(client);
			npc->FirstSeen(_id);
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

	std::unordered_set<uint64> closed_clients = server.GetClientList(_position);
	for (uint64 client_id : closed_clients) {

		if (client_id == _id) continue;	// 내 ID라면 무시

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;	// 게임 참여 중 아니라면 무시

		// view list 판단
		if (client->CanSee(_position, VIEW_RANGE)) {	// 보이는 경우
			near_list.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, _level };		// TODO: 값 제대로
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y };

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
			if (client->IsNPC()) {
				auto npc = static_cast<Bot*>(client);
				npc->FirstSeen(_id);
			}

			Position pos = client->GetPosition();
			SC_ENTER_PACKET object_enter_packet{ client_id, pos.x, pos.y, client->GetName().data(), 0, _level };		// TODO: 값 제대로

			SendNewCreature(client_id, &object_enter_packet);
		}
	}

	// view_list 벗어났다면 leave_packet 전송
	for (uint64 client_id : old_list) {

		if (client_id == _id) continue;	// 내 ID라면 무시 위에서 거르지만 혹시 모르니깐

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		if (near_list.count(client_id) == 0) {
			if (client->IsPlayer()) {
				auto session = static_cast<Session*>(client);
				session->SendLeaveCreature(_id);
			}
			SendLeaveCreature(client_id);
		}
	}
}

void Session::ChatProcess(BYTE* packet)
{
	CS_CHAT_PACKET* chat_packet = reinterpret_cast<CS_CHAT_PACKET*>(packet);

	SC_CHAT_PACKET chat_broadcast_packet{ _id, chat_packet->_message };

	_view_lock.lock();
	std::unordered_set<uint64> now_list = _view_list;
	_view_lock.unlock();

	for (uint64 client_id : now_list) {

		if (client_id == _id) continue;	// 내 ID라면 무시
		if (::IsNPC(client_id)) continue;	// NPC 라면 무시

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;	// 게임 참여 중 아니라면 무시

		Session* session = static_cast<Session*>(client);
		session->Send(&chat_broadcast_packet);
	}
}

void Session::AttackProcess(BYTE* packet)
{
	CS_ATTACK_PACKET* attack_packet = reinterpret_cast<CS_ATTACK_PACKET*>(packet);

	uint8 attack_type = AttackType::ATTACK_NONE;

	switch (attack_packet->_atk_key)
	{
	case KeyType::KEY_A:
		attack_type = AttackType::ATTACK_NONE;
		break;
	case KeyType::KEY_S:
	{

	}
	break;

	case KeyType::KEY_D:
	{

	}
	break;

	default:
	{
		std::cout << "Attack Process Key Type Error\n";
		exit(-1);
	}
	break;
	}


	SC_ATTACK_PACKET attack_broadcast_packet{ _id, attack_type };

	_view_lock.lock();
	std::unordered_set<uint64> now_list = _view_list;
	_view_lock.unlock();

	for (uint64 client_id : now_list) {

		if (client_id == _id) continue;		// 내 ID라면 무시
		if (::IsNPC(client_id)) continue;	// NPC 라면 무시

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;	// 게임 참여 중 아니라면 무시

		Session* session = static_cast<Session*>(client);
		session->Send(&attack_broadcast_packet);
	}

	Position pos = _position;
	SC_DAMAGE_PACKET damage_packet{};
	std::unordered_set<uint64> closed_clients = server.GetClientList(pos);
	switch (attack_type)
	{
	case AttackType::STANDARD_ATK:
	{
		uint16 damage = 20;

		for (uint64 client_id : closed_clients) {

			if (client_id == _id) continue;		// 내 ID라면 무시

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;	// nullptr 이라면 무시

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE or state == GameState::ST_DEAD) continue;	// 게임 참여 중 아니라면 무시

			Position client_pos = client->GetPosition();
			int16 dx = std::abs(client_pos.x - pos.x);
			int16 dy = std::abs(client_pos.y - pos.y);
			if ((dx + dy) <= 1) {
				// 맞은 것임

				client->TakeDamage(damage);
				damage_packet.AddDamageInfo(client_id, damage);
			}
		}
	}
	break;

	// TODO: 공격 타입에 따라 공격 완성하기

	}

	Send(&damage_packet);
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
	SC_MOVE_SELF_PACKET update_packet{ _position.x, _position.y, _last_move_time };

	Send(&update_packet);
}
