#pragma once


class Monster : public Bot
{
public:
	Monster();
	Monster(uint64 id);
	Monster(uint64 id, bool neut, bool peac, bool fix);
	~Monster();

};
