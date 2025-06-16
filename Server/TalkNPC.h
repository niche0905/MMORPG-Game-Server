#pragma once


class TalkNPC : public Bot
{
public:
	TalkNPC();
	TalkNPC(uint64 id);
	~TalkNPC();

	void Update() override;
	void FirstSeen(uint64 id) override;

};

