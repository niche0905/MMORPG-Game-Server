#include "pch.h"
#include "TalkNPC.h"


TalkNPC::TalkNPC()
	: TalkNPC{ INVALID_ID }
{

}

TalkNPC::TalkNPC(uint64 id)
	: Bot{id, ClassType::TALK_NPC, true, true, true, true, true}
{
	_visual_type = VisualInfo::VI_MONSTER;
}

TalkNPC::~TalkNPC()
{

}

void TalkNPC::Update()
{
	using namespace std::chrono;

	if (ClosePlayer()) {
		server.AddTimerEvent(Event{ _id, system_clock::now() + 5s, Event::EventType::EV_UPDATE });
	}
	else {
		DisableActive();
	}
}

void TalkNPC::FirstSeen(uint64 id)
{
	Creature* client = server.GetClients()[id];
	if (client == nullptr) return;

	uint8 state = client->GetState();
	if (state == GameState::ST_ALLOC or state == GameState::ST_CLOSE) return;

	if (not client->CanSee(_position, VIEW_RANGE)) return;

	std::string chat = "Hello";
	SC_CHAT_PACKET chat_packet{ _id, chat.c_str() };

	Session* session = static_cast<Session*>(client);
	session->Send(&chat_packet);

}
