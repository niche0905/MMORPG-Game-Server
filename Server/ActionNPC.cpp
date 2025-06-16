#include "pch.h"
#include "ActionNPC.h"


ActionNPC::ActionNPC()
	: ActionNPC{ INVALID_ID }
{

}

ActionNPC::ActionNPC(uint64 id)
	: Bot{ id, ClassType::KNIGHT_NPC, true, true, true, true, true }
{
	_is_action = true;
	_visual_type = VisualInfo::VI_MONSTER;
}

ActionNPC::~ActionNPC()
{

}

void ActionNPC::Update()
{
	// Update에서 할 게 없다
}

void ActionNPC::DoAction(uint64 id)
{
	if (::IsNPC(id)) return;

	Creature* client = server.GetClients()[id];
	if (client == nullptr) return;

	uint8 state = client->GetState();
	if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) return;

	if (not client->CanSee(_position, VIEW_RANGE)) return;

	if (state == GameState::ST_DEAD) return;

	Session* session = static_cast<Session*>(client);

	SC_DIALOG_PACKET dialog_packet{};
	session->Send(&dialog_packet);
}

bool ActionNPC::CanAction(Position pos)
{
	int16 dx = std::abs(pos.x - _position.x);
	int16 dy = std::abs(pos.y - _position.y);
	return (dx <= 1 and dy <= 1);
}
