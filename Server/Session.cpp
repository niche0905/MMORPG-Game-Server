#include "pch.h"
#include "Session.h"
#include "ActionNPC.h"


Session::Session()
	: Session{ 0, INVALID_ID }
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
	using namespace std::chrono;

	_class_type = ClassType::CLASS_NONE;
	_move_cooltime = steady_clock::now() - MOVE_COOLTIME;
	_aatk_cooltime = steady_clock::now() - AATK_COOLTIME;
	_satk_cooltime = steady_clock::now() - SATK_COOLTIME;
	_datk_cooltime = steady_clock::now() - DATK_COOLTIME;
}

Session::~Session()
{
	Disconnect();
}

void Session::Disconnect()
{
	if (_state != ST_CLOSE) {

		if (_state != GameState::ST_ALLOC) {

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

			server.AddRequestDB(DatabaseEvent{ _id, DatabaseEvent::DbOperation::DB_LOGOUT_PROCESS });
		}
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

	using namespace std::chrono;

	_class_type = ClassType::CLASS_NONE;
	_move_cooltime = steady_clock::now() - MOVE_COOLTIME;
	_aatk_cooltime = steady_clock::now() - AATK_COOLTIME;
	_satk_cooltime = steady_clock::now() - SATK_COOLTIME;
	_datk_cooltime = steady_clock::now() - DATK_COOLTIME;
}

bool Session::IsPlayer() const
{
	return true;
}

bool Session::IsNPC() const
{
	return false;
}

void Session::SetLevel(uint8 level)
{
	_level = level;
}

uint64 Session::GetExp() const
{
	return _exp;
}

Stats Session::GetStats() const
{
	return Creature::GetStats() + GetEquipStats();
}

Stats Session::GetEquipStats() const
{
	return Stats();		// TODO 구현 하여야 함
}

bool Session::TakeDamage(uint64 id, uint16 damage)
{
	if (_state == GameState::ST_DEAD) return false;

	bool my_kill = Creature::TakeDamage(id, damage);

	SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };

	_view_lock.lock();
	std::unordered_set<uint64> view_list = _view_list;
	_view_lock.unlock();

	Send(&hp_change_packet);
	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&hp_change_packet);
	}

	if (my_kill) {
		DeadSequence();
	}

	NeedGeneration();

	return my_kill;
}

void Session::DeadSequence()
{
	SC_DEAD_PACKET dead_packet{};
	_visual_type = VisualInfo::VI_GRAVE;
	SC_UPDATE_VI_PACKET update_vi_packet{ _id, _visual_type };

	_view_lock.lock();
	std::unordered_set<uint64> view_list = _view_list;
	_view_lock.unlock();

	Send(&dead_packet);
	Send(&update_vi_packet);
	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&update_vi_packet);
	}
}

void Session::UpdateVisualInfo()
{
	switch (_class_type)
	{
	case ClassType::WARRIOR:
		_visual_type = VisualInfo::VI_WARRIOR;
		break;
	case ClassType::ROGUE:
		_visual_type = VisualInfo::VI_ROGUE;
		break;
	case ClassType::SORCERER:
		_visual_type = VisualInfo::VI_SORCERER;
		break;
	}
}

void Session::SetClassType(uint8 class_type)
{
	Creature::SetClassType(class_type);
	UpdateVisualInfo();
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
			//PrintErrorMessage(L"WSASend", error);
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
			//PrintErrorMessage(L"WSARecv", error);
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

	case PacketID::C2S_REGISTER:
	{
		RegisterProcess(packet);
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

	case PacketID::C2S_TELEPORT:
	{
		TeleportProcess(packet);
	}
	break;
	
	case PacketID::C2S_RESPAWN:
	{
		RespawnProcess(packet);
	}
	break;

	case PacketID::C2S_INTERACTION:
	{
		InteractionProcess(packet);
	}
	break;

	case PacketID::C2S_SET_BASE_POS:
	{
		SetBasePosProcess(packet);
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

	DatabaseEvent db_event{ _id, DatabaseEvent::DbOperation::DB_LOGIN_REQUEST };
	server.AddRequestDB(db_event);
}

void Session::RegisterProcess(BYTE* packet)
{
	CS_REGISTER_PACKET* register_packet = reinterpret_cast<CS_REGISTER_PACKET*>(packet);
	SetName(register_packet->_name);

	_class_type = register_packet->_class_type;
	// TODO: 적절한 위치에 생성해야 함

	Position spawn_pos;
	while (true) {
		spawn_pos = { rand() % MAX_WIDTH, rand() % MAX_HEIGHT };
		if (not IsBlock(spawn_pos))
			break;
	}
	_position = spawn_pos;

	DatabaseEvent db_event{ _id, DatabaseEvent::DbOperation::DB_REGISTER_REQUEST };
	server.AddRequestDB(db_event);
}

void Session::MoveProcess(BYTE* packet)
{
	if (_state == GameState::ST_DEAD) return;

	using namespace std::chrono;

	auto now_point = steady_clock::now();
	if (server.IsDev());
	else {
		uint16 mover = GetStats().MOV;
		if ((now_point - _move_cooltime) < (MOVE_COOLTIME - GRACE_TIME - milliseconds(mover))) {
			return;		// 이동 실패
		}
	}
	_move_cooltime = now_point;

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

	if (::IsBlock(new_position) or _position == new_position) {
		// 벽에 막혀서 못움직여요		변한게 없다면 스스로에게만 시간 Update
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

		if (client->IsNPC() and state == GameState::ST_DEAD) continue;

		// view list 판단
		if (client->CanSee(_position, VIEW_RANGE)) {	// 보이는 경우
			near_list.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.c_str(),  GetMaxHP(), _hp, _visual_type, _class_type, _level };
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
			SC_ENTER_PACKET object_enter_packet{ client_id, pos.x, pos.y, client->GetName().c_str(), client->GetMaxHP(), client->GetHP(), client->GetVisualType(), client->GetClassType(), client->GetLevel()};

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

	Send(&chat_broadcast_packet);
	for (uint64 client_id : now_list) {

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
	if (_state == GameState::ST_DEAD) return;

	using namespace std::chrono;

	auto now_point = steady_clock::now();

	CS_ATTACK_PACKET* attack_packet = reinterpret_cast<CS_ATTACK_PACKET*>(packet);

	uint8 attack_type = AttackType::ATTACK_NONE;

	switch (attack_packet->_atk_key)
	{
	case KeyType::KEY_A:
		attack_type = AttackType::STANDARD_ATK;

		if (server.IsDev());
		else {
			if ((now_point - _aatk_cooltime) < (AATK_COOLTIME - GRACE_TIME)) {
				return;		// A공격 실패
			}
		}
		_aatk_cooltime = now_point;
		break;
	case KeyType::KEY_S:
	{
		switch (_class_type)
		{
		case ClassType::WARRIOR:
			attack_type = AttackType::WARRIOR_S;
			break;
		case ClassType::ROGUE:
			attack_type = AttackType::ROGUE_S;
			break;
		case ClassType::SORCERER:
			attack_type = AttackType::SORCERER_S;
			break;
		}

		if (server.IsDev());
		else {
			if ((now_point - _satk_cooltime) < (SATK_COOLTIME - GRACE_TIME)) {
				return;		// S공격 실패
			}
		}
		_satk_cooltime = now_point;
	}
	break;

	case KeyType::KEY_D:
	{

		if (server.IsDev());
		else {
			if ((now_point - _datk_cooltime) < (DATK_COOLTIME - GRACE_TIME)) {
				return;		// D공격 실패
			}
		}
		_datk_cooltime = now_point;
	}
	break;

	default:
	{
		std::cout << "Attack Process Key Type Error\n";
		exit(-1);
	}
	break;
	}


	SC_ATTACK_PACKET attack_broadcast_packet{ _id, attack_packet->_atk_key, attack_packet->_atk_dir };

	_view_lock.lock();
	std::unordered_set<uint64> now_list = _view_list;
	_view_lock.unlock();

	Send(&attack_broadcast_packet);
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


	Stats my_stats = GetStats();
	uint16 addition_damage = 0;
	switch (_class_type)
	{
	case ClassType::WARRIOR:
		addition_damage = static_cast<uint16>(my_stats.ATK + (my_stats.STR / 2.f) + (my_stats.DEX / 4.f) + (my_stats.INT / 4.f));
		break;
	case ClassType::ROGUE:
		addition_damage = static_cast<uint16>(my_stats.ATK + (my_stats.STR / 4.f) + (my_stats.DEX / 2.f) + (my_stats.INT / 4.f));
		break;
	case ClassType::SORCERER:
		addition_damage = static_cast<uint16>(my_stats.ATK + (my_stats.STR / 4.f) + (my_stats.DEX / 4.f) + (my_stats.INT / 2.f));
		break;
	}
	float critical = 1.f;
	if ((rand() % 100) < static_cast<int>(my_stats.CRT / 150.f)) {
		critical = 1.5f;
	}

	Position pos = _position;
	SC_DAMAGE_PACKET damage_packet{};
	std::unordered_set<uint64> closed_clients = server.GetClientList(pos);
	switch (attack_type)
	{
	case AttackType::STANDARD_ATK:
	{
		uint16 damage = 20;
		damage += addition_damage;
		damage = static_cast<uint16>(damage * critical);

		for (uint64 client_id : closed_clients) {

			if (client_id == _id) continue;		// 내 ID라면 무시

			if (::IsPlayer(client_id)) continue;	// 플레이어라면 무시 (플레이어 끼린 평화?? 이지 않나?)

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;	// nullptr 이라면 무시

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE or state == GameState::ST_DEAD) continue;	// 게임 참여 중 아니라면 무시

			Bot* npc = static_cast<Bot*>(client);
			if (npc->IsFriendly() or npc->IsInvincibility()) continue;		// 우호관계, 무적이라면 스킵

			// 공격 범위 판정!
			Position client_pos = client->GetPosition();
			int16 dx = std::abs(client_pos.x - pos.x);
			int16 dy = std::abs(client_pos.y - pos.y);
			if ((dx + dy) <= 1) {
				// 맞은 것임

				if (client->TakeDamage(_id, damage))
					npc->DropItem(_id);
				if (damage_packet._num < SC_DAMAGE_PACKET::MAX_DAMAGE_INFO_NUM)
					damage_packet.AddDamageInfo(client_id, damage);
			}
		}
	}
	break;

	case AttackType::WARRIOR_S:
	{
		uint16 damage = 40;
		damage += addition_damage;
		damage = static_cast<uint16>(damage * critical);

		for (uint64 client_id : closed_clients) {

			if (client_id == _id) continue;

			if (::IsPlayer(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE or state == GameState::ST_DEAD) continue;

			Bot* npc = static_cast<Bot*>(client);
			if (npc->IsFriendly() or npc->IsInvincibility()) continue;

			// 공격 범위 판정!
			Position client_pos = client->GetPosition();
			int16 dx = std::abs(client_pos.x - pos.x);
			int16 dy = std::abs(client_pos.y - pos.y);
			if (dx <= 2 and dy <= 2) {
				// 맞은 것임

				if (client->TakeDamage(_id, damage))
					npc->DropItem(_id);
				if (damage_packet._num < SC_DAMAGE_PACKET::MAX_DAMAGE_INFO_NUM)
					damage_packet.AddDamageInfo(client_id, damage);
			}
		}
	}
	break;

	case AttackType::ROGUE_S:
	{
		uint16 damage = 100;
		damage += addition_damage;
		damage = static_cast<uint16>(damage * critical);

		std::vector<Position> hit_pos;
		hit_pos.reserve(2);
		hit_pos.emplace_back(pos);
		switch (attack_packet->_atk_dir)
		{
		case DIR_LEFT:
			hit_pos.emplace_back(pos.x - 1, pos.y);
			break;
		case DIR_RIGHT:
			hit_pos.emplace_back(pos.x + 1, pos.y);
			break;
		case DIR_UP:
			hit_pos.emplace_back(pos.x, pos.y - 1);
			break;
		case DIR_DOWN:
			hit_pos.emplace_back(pos.x, pos.y + 1);
			break;
		}

		for (uint64 client_id : closed_clients) {

			if (client_id == _id) continue;

			if (::IsPlayer(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE or state == GameState::ST_DEAD) continue;

			Bot* npc = static_cast<Bot*>(client);
			if (npc->IsFriendly() or npc->IsInvincibility()) continue;

			// 공격 범위 판정!
			Position client_pos = client->GetPosition();
			
			for (auto& can_pos : hit_pos) {
				if (can_pos == client_pos) {
					// 맞은 것임

					if (client->TakeDamage(_id, damage))
						npc->DropItem(_id);
					if (damage_packet._num < SC_DAMAGE_PACKET::MAX_DAMAGE_INFO_NUM)
						damage_packet.AddDamageInfo(client_id, damage);
					break;
				}
			}
		}
	}
	break;

	case AttackType::SORCERER_S:
	{
		uint16 damage = 60;
		damage += addition_damage;
		damage = static_cast<uint16>(damage * critical);

		std::vector<Position> hit_pos;
		hit_pos.reserve(6);
		switch (attack_packet->_atk_dir)
		{
		case DIR_LEFT:
			for (int i = 0; i < 6; ++i) {
				hit_pos.emplace_back(pos.x - i, pos.y);
			}
			break;
		case DIR_RIGHT:
			for (int i = 0; i < 6; ++i) {
				hit_pos.emplace_back(pos.x + i, pos.y);
			}
			break;
		case DIR_UP:
			for (int i = 0; i < 6; ++i) {
				hit_pos.emplace_back(pos.x, pos.y - i);
			}
			break;
		case DIR_DOWN:
			for (int i = 0; i < 6; ++i) {
				hit_pos.emplace_back(pos.x, pos.y + i);
			}
			break;
		}

		for (uint64 client_id : closed_clients) {

			if (client_id == _id) continue;

			if (::IsPlayer(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE or state == GameState::ST_DEAD) continue;

			Bot* npc = static_cast<Bot*>(client);
			if (npc->IsFriendly() or npc->IsInvincibility()) continue;

			// 공격 범위 판정!
			Position client_pos = client->GetPosition();

			for (auto& can_pos : hit_pos) {
				if (can_pos == client_pos) {
					// 맞은 것임

					if (client->TakeDamage(_id, damage))
						npc->DropItem(_id);
					if (damage_packet._num < SC_DAMAGE_PACKET::MAX_DAMAGE_INFO_NUM)
						damage_packet.AddDamageInfo(client_id, damage);
					break;
				}
			}
		}
	}
	break;

	// TODO: 공격 타입에 따라 공격 완성하기

	}

	if (damage_packet._num != 0)	// 맞은 사람이 없으면 보낼 이유가 없음
		Send(&damage_packet);
}

void Session::TeleportProcess(BYTE* packet)
{
	if (_state == GameState::ST_DEAD) return;

	CS_TELEPORT_PACKET* tp_packet = reinterpret_cast<CS_TELEPORT_PACKET*>(packet);

	Position new_pos = { tp_packet->_x, tp_packet->_y };

	if (not ::IsValid(new_pos)) return;

	if (::IsBlock(new_pos) or _position == new_pos) return;

	server.MoveSector(_id, _position, new_pos);

	_position = new_pos;
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

		if (client->IsNPC() and state == GameState::ST_DEAD) continue;

		// view list 판단
		if (client->CanSee(_position, VIEW_RANGE)) {	// 보이는 경우
			near_list.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.c_str(),  GetMaxHP(), _hp, _visual_type, _class_type, _level };
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
			SC_ENTER_PACKET object_enter_packet{ client_id, pos.x, pos.y, client->GetName().c_str(), client->GetMaxHP(), client->GetHP(), client->GetVisualType(), client->GetClassType(), client->GetLevel() };

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

void Session::RespawnProcess(BYTE* packet)
{
	if (_state != GameState::ST_DEAD) {
		std::cout << "not dead respawn request ERROR!!!\n";
		return;
	}

	UpdateVisualInfo();

	_hp = GetMaxHP();
	Position old_pos = _position;

	server.MoveSector(_id, _position, _base_pos);
	_position = _base_pos;

	_state = GameState::ST_ALIVE;

	SC_REVIVE_PACKET revive_packet{};
	Send(&revive_packet);
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

		if (client->IsNPC() and state == GameState::ST_DEAD) continue;

		// view list 판단
		if (client->CanSee(_position, VIEW_RANGE)) {	// 보이는 경우
			near_list.insert(client_id);
		}
	}

	SC_UPDATE_VI_PACKET vi_packet{ _id, _visual_type };
	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.c_str(),  GetMaxHP(), _hp, _visual_type, _class_type, _level };
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y };
	SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };

	Send(&vi_packet);
	Send(&hp_change_packet);
	for (uint64 client_id : near_list) {

		if (client_id == _id) continue;	// 내 ID라면 무시 위에서 거르지만 혹시 모르니깐

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		if (client->IsPlayer()) {
			auto session = static_cast<Session*>(client);
			session->ProcessCloseCreature(_id, &enter_packet, &update_packet, &vi_packet);
			session->Send(&hp_change_packet);	// 꼭 불러야 하는 것은 아닌데 아쉽다; 위 vi_packet과 같다
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
			SC_ENTER_PACKET object_enter_packet{ client_id, pos.x, pos.y, client->GetName().c_str(), client->GetMaxHP(), client->GetHP(), client->GetVisualType(), client->GetClassType(), client->GetLevel() };

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

void Session::InteractionProcess(BYTE* packet)
{
	std::unordered_set<uint64> closed_clients = server.GetClientList(_position);
	for (uint64 client_id : closed_clients) {

		if (client_id == _id) continue;	// 내 ID라면 무시

		if (::IsPlayer(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;	// nullptr 이라면 무시

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE and state == GameState::ST_DEAD) continue;	// 게임 참여 중 아니라면 무시

		Bot* npc = static_cast<Bot*>(client);
		if (not npc->IsAction()) continue;

		ActionNPC* a_npc = static_cast<ActionNPC*>(npc);
		if (a_npc->CanAction(_position)) {
			a_npc->DoAction(_id);
		}
	}
}

void Session::SetBasePosProcess(BYTE* packet)
{
	_base_pos = _position;
}

uint64 Session::GetUserID() const
{
	return _user_id;
}

bool Session::LoginInfo(uint64 user_id, int16 x, int16 y, uint16 maxHP, uint16 HP, uint8 class_type, uint32 level, uint64 exp)
{
	_user_id = user_id;

	if (server.GetMappingTable().count(user_id) != 0) {
		if (server.GetMappingTable()[user_id] != INVALID_ID) {
			SC_LOGIN_FAIL_PACKET login_fail_packet{ LoginFailReason::USED_ID };
			Send(&login_fail_packet);

			return false;
		}
	}

	server.GetMappingTable()[_user_id] = _id;

	SetPosition(x, y);
	server.RegisterSector(_id, _position);
	_hp = HP;
	SetClassType(class_type);
	_level = level;
	_exp = exp;

	return true;
}

void Session::LoginDone()
{
	_base_pos = _position;
	server.GetMappingTable()[_user_id] = _id;
	_state = GameState::ST_INGAME;

	NeedGeneration();

	SC_LOGIN_ALLOW_PACKET login_allow_packet{ _id, _position.x, _position.y, GetMaxHP(), _hp, _visual_type, _class_type, _level, _exp };
	Send(&login_allow_packet);
	StatsChange();

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.c_str(), GetMaxHP(), _hp, _visual_type, _class_type, _level };
	SC_MOVE_PACKET move_packet{ _id, _position.x, _position.y };

	std::unordered_set<uint64> user_list = server.GetClientList(_position);
	for (uint64 client_id : user_list) {
		if (client_id == _id) continue;

		auto client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (client->IsNPC() and state == GameState::ST_DEAD) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Position client_pos = client->GetPosition();
		SC_ENTER_PACKET other_enter_packet{ client_id, client_pos.x, client_pos.y, client->GetName().c_str(),  client->GetMaxHP(), client->GetHP(), client->GetVisualType(), client->GetClassType(), client->GetLevel()};
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

void Session::LoginFalse()
{
	SC_LOGIN_FAIL_PACKET login_fail_packet{ LoginFailReason::GO_REGISTER };
	Send(&login_fail_packet);
}

bool Session::RegisterInfo(uint64 user_id)
{
	_user_id = user_id;

	if (server.GetMappingTable().count(user_id) != 0) {
		if (server.GetMappingTable()[user_id] != INVALID_ID) {
			SC_LOGIN_FAIL_PACKET login_fail_packet{ LoginFailReason::USED_ID };
			Send(&login_fail_packet);

			return false;
		}
	}

	server.GetMappingTable()[_user_id] = _id;

	server.RegisterSector(_id, _position);

	return true;
}

void Session::RegisterFalse()
{
	SC_LOGIN_FAIL_PACKET login_fail_packet{ LoginFailReason::USED_ID };
	Send(&login_fail_packet);
}

void Session::ProcessCloseCreature(uint64 id, void* enter_packet, void* move_packet, void* vi_packet)
{
	_view_lock.lock();
	if (_view_list.count(id) == 0) {	// 기존에 없던 생명체
		_view_list.insert(id);
		_view_lock.unlock();

		Send(enter_packet);
	}
	else {								// 기존에 있던 생명체
		_view_lock.unlock();
		if (vi_packet)
			Send(vi_packet);
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

void Session::HealSelf10()
{
	const uint16 healed_hp = GetMaxHP() / 10;

	uint16 expected;
	uint16 desired;
	do {
		uint16 max_hp = GetMaxHP();

		expected = _hp.load();
		if (expected <= 0 or expected >= max_hp) {
			return;
		}

		int32 calc = static_cast<int32>(expected) + static_cast<int32>(healed_hp);
		desired = static_cast<uint16>(std::min<int32>(calc, max_hp));
	} while (not _hp.compare_exchange_strong(expected, desired));

	SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };

	_view_lock.lock();
	std::unordered_set<uint64> view_list = _view_list;
	_view_lock.unlock();

	Send(&hp_change_packet);
	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&hp_change_packet);
	}

	if (_hp < GetMaxHP()) {
		using namespace std::chrono;

		Event evt{ _id, std::chrono::system_clock::now() + 10s, Event::EventType::EV_HEAL };
		server.AddTimerEvent(evt);
	}
	else {
		_is_timed = false;
	}
}

void Session::AddExp(uint64 exp)
{
	if (_exp > UINT64_MAX - exp) {
		_exp = UINT64_MAX;		// 오버 플로우 발생;;
	}
	else {
		_exp += exp;
	}

	bool flag = false;
	while (_level < MAX_LEVEL) {

		uint64 need_exp = ::NeedExpToLevelUp(_level);
		if (_exp >= need_exp) {
			_exp -= need_exp;

			++_level;
			_hp = GetMaxHP();
			flag = true;
		}
		else break;

	}

	if (flag) {

		SC_STATS_CHANGE_PACKET sc_packet{ GetStats() };
		Send(&sc_packet);

		SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };
		SC_LEVEL_CHANGE_PACKET level_chabge_packet{ _id, _level };

		_view_lock.lock();
		std::unordered_set<uint64> view_list = _view_list;
		_view_lock.unlock();

		Send(&hp_change_packet);
		Send(&level_chabge_packet);
		for (uint64 client_id : view_list) {

			if (::IsNPC(client_id)) continue;

			Creature* client = server.GetClients()[client_id];
			if (client == nullptr) continue;

			uint8 state = client->GetState();
			if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

			Session* session = static_cast<Session*>(client);
			session->Send(&hp_change_packet);
			session->Send(&level_chabge_packet);
		}
	}

	SC_EXP_UP_PACKET exp_change{ _exp };
	Send(&exp_change);
}

void Session::SelfUpdate()
{
	SC_MOVE_SELF_PACKET update_packet{ _position.x, _position.y, _last_move_time };

	Send(&update_packet);
}

void Session::StatsChange()
{
	SC_STATS_CHANGE_PACKET stats_change_packet{ GetStats() };
	Send(&stats_change_packet);
}

void Session::NeedGeneration()
{
	using namespace std::chrono;

	if (_hp < GetMaxHP()) {

		bool expected = false;
		if (_is_timed.compare_exchange_strong(expected, true)) {

			Event evt{ _id, std::chrono::system_clock::now() + 10s, Event::EventType::EV_HEAL };
			server.AddTimerEvent(evt);
		}
	}
}
