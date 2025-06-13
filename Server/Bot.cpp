#include "pch.h"
#include "Bot.h"
#include "IdleState.h"
#include "DeadState.h"


Bot::Bot()
{
	std::cout << "Bot 기본 생성자 호출 에러\n";
	exit(-1);
}

Bot::Bot(uint64 id)
	: Bot{ id, BOT_START, false, true, false, false, false }
{

}

Bot::Bot(uint64 id, uint8 b_type, bool invin, bool neut, bool peace, bool firend, bool fix)
	: Creature{ id, false }
	, _fsm{}
	, _target{ nullptr }
	, _bot_type{ b_type }
	, _is_invincibility{ invin }
	, _is_neutrality{ neut }
	, _is_peace{ peace }
	, _is_friendly{ firend }
	, _is_fix{ fix }
{
	_fsm.ChangeState(this, &IdleState::Instance());
}

Bot::~Bot()
{

}

bool Bot::IsPlayer() const
{
	return false;
}

bool Bot::IsNPC() const
{
	return true;
}

uint16 Bot::GetMaxHP() const
{
	return _basic_stats.HP + _temp_stats.HP;
}

void Bot::TakeDamage(uint16 damage)
{
	Creature::TakeDamage(damage);

	SC_HP_CHANGE_PACKET hp_change_packet{ _id, _hp };

	std::unordered_set<uint64> view_list = server.GetClientList(_position);

	for (uint64 client_id : view_list) {

		if (client_id == _id or ::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&hp_change_packet);
	}
}

void Bot::DeadSequence()
{
	std::unordered_set<uint64> view_list = server.GetClientList(_position);

	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Session* session = static_cast<Session*>(client);
		session->SendLeaveCreature(_id);
	}
}

uint8 Bot::GetBotType() const
{
	return _bot_type;
}

bool Bot::GetInvincibility() const
{
	return _is_invincibility;
}

bool Bot::GetNeutrality() const
{
	return _is_neutrality;
}

bool Bot::GetPeace() const
{
	return _is_peace;
}

bool Bot::GetFriendly() const
{
	return false;
}

bool Bot::GetFix() const
{
	return _is_fix;
}

void Bot::Update()
{
	// TODO: 조건에 따라서 change state
	if (_state == GameState::ST_DEAD) {
		_fsm.ChangeState(this, &DeadState::Instance());
		return;
	}

	_fsm.Update(this);
}

void Bot::FirstSeen(uint64 id)
{

}

void Bot::WakeUp()
{
	if (_is_active) return;
	bool old_state = false;
	if (_is_active.compare_exchange_strong(old_state, true) == false) return;

	Event evt{ _id, std::chrono::system_clock::now(), Event::EventType::EV_RANDOM_MOVE };
	server.AddTimerEvent(evt);
}

void Bot::DoRandomMove()
{
	using namespace std::chrono;

	Position new_position = _position;
	switch (rand() % 4 + 1)
	{
	case MOVE_UP: 
		if (new_position.y > 0) 
			--new_position.y; 
		break;
	case MOVE_DOWN: 
		if (new_position.y < (MAX_HEIGHT - 1)) 
			++new_position.y; 
		break;
	case MOVE_LEFT: 
		if (new_position.x > 0) 
			--new_position.x; 
		break;
	case MOVE_RIGHT: 
		if (new_position.x < (MAX_WIDTH - 1)) 
			++new_position.x; 
		break;
	}

	if (_position == new_position) {

		Event evt{ _id, system_clock::now() + 1s, Event::EventType::EV_RANDOM_MOVE };
		server.AddTimerEvent(evt);

		return;
	}
	
	std::unordered_set<uint64> old_view;
	std::unordered_set<uint64> old_close_id = server.GetClientList(_position);
	for (uint64 client_id : old_close_id) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == ST_ALLOC or state == ST_CLOSE) continue;

		if (client->CanSee(_position, VIEW_RANGE)) {
			old_view.insert(client_id);
		}
	}

	server.MoveSector(_id, _position, new_position);
	_position = new_position;

	std::unordered_set<uint64> new_view;
	std::unordered_set<uint64> new_close_id = server.GetClientList(_position);
	for (uint64 client_id : new_close_id) {
		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == ST_ALLOC or state == ST_CLOSE) continue;

		if (client->CanSee(_position, VIEW_RANGE)) {
			new_view.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, _level };
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y };

	for (uint64 client_id : new_view) {
		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;
		Session* session = static_cast<Session*>(client);
		session->ProcessCloseCreature(_id, &enter_packet, &update_packet);
	}

	for (uint64 client_id : old_view) {
		if (new_view.count(client_id) != 0) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;
		Session* session = static_cast<Session*>(client);
		session->SendLeaveCreature(_id);
	}

	if (new_view.size() == 0) {
		_is_active = false;
		return;
	}

	Event evt{ _id, system_clock::now() + 1s, Event::EventType::EV_RANDOM_MOVE };
	server.AddTimerEvent(evt);
}

void Bot::DoRevive()
{
	std::cout << "DoRevive\n";

	_state = GameState::ST_ALIVE;
	_fsm.ChangeState(this, &IdleState::Instance());

	std::unordered_set<uint64> view_list = server.GetClientList(_position);
	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), _visual_type, _level };

	for (uint64 client_id : view_list) {

		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		uint8 state = client->GetState();
		if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) continue;

		if (not client->CanSee(_position, VIEW_RANGE)) continue;

		Session* session = static_cast<Session*>(client);
		session->Send(&enter_packet);
	}
}
