#include "pch.h"
#include "Bot.h"


Bot::Bot()
{
	std::cout << "Bot 기본 생성자 호출 에러\n";
	exit(-1);
}

Bot::Bot(int64 id)
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
