#include "pch.h"
#include "Bot.h"


Bot::Bot()
{
	std::cout << "Bot 기본 생성자 호출 에러\n";
	exit(-1);
}

Bot::Bot(uint64 id)
	: Creature{ id, false }
{

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
	for (uint64 client_id : server.GetClientList(_position)) {
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
	for (uint64 client_id : server.GetClientList(_position)) {
		if (::IsNPC(client_id)) continue;

		Creature* client = server.GetClients()[client_id];
		if (client == nullptr) continue;

		auto state = client->GetState();
		if (state == ST_ALLOC or state == ST_CLOSE) continue;

		if (client->CanSee(_position, VIEW_RANGE)) {
			new_view.insert(client_id);
		}
	}

	SC_ENTER_PACKET enter_packet{ _id, _position.x, _position.y, _name.data(), 0, 1 };		// TODO: 값 제대로
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
