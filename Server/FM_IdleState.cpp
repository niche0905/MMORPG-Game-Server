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
	server.AddTimerEvent(Event{ bot->GetID(), system_clock::now() + 1s, Event::EventType::EV_UPDATE });
}

void FM_IdleState::Exit(Bot* bot)
{

}
