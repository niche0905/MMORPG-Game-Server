#include "pch.h"
#include "Bot.h"


Bot::Bot()
{
	std::cout << "Bot �⺻ ������ ȣ�� ����\n";
	exit(-1);
}

Bot::Bot(int64 id)
	: BaseSession{ id }
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
