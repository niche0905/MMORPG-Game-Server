#include "pch.h"
#include "DeadState.h"


DeadState& DeadState::Instance()
{
	static DeadState instance;
	return instance;
}

void DeadState::Enter(Bot* bot)
{
	using namespace std::chrono;

	bot->DeadSequence();
	// 30초 후에 부활하도록
	server.AddTimerEvent(Event{ bot->GetID(), system_clock::now() + 30s, Event::EventType::EV_REVIVE });
}

void DeadState::Execute(Bot* bot)
{

}

void DeadState::Exit(Bot* bot)
{
	
}
