#include "pch.h"
#include "FM_IdleState.h"


FM_IdleState& FM_IdleState::Instance()
{
	static FM_IdleState instance;
	return instance;
}

void FM_IdleState::Enter(Bot* bot)
{

}

void FM_IdleState::Execute(Bot* bot)
{
	using namespace std::chrono;

	if (bot->ClosePlayer()) {
		server.AddTimerEvent(Event{ bot->GetID(), system_clock::now() + 1s, Event::EventType::EV_UPDATE });
	}
	else {
		bot->DisableActive();
	}
}

void FM_IdleState::Exit(Bot* bot)
{

}
