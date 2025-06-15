#include "pch.h"
#include "AttackState.h"


AttackState& AttackState::Instance()
{
	static AttackState instance;
	return instance;
}

void AttackState::Enter(Bot* bot)
{

}

void AttackState::Execute(Bot* bot)
{
	using namespace std::chrono;

	bot->Attack();
	server.AddTimerEvent(Event{ bot->GetID(), system_clock::now() + 1s, Event::EventType::EV_UPDATE });
}

void AttackState::Exit(Bot* bot)
{

}
