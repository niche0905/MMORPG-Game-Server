#pragma once


class MerchantNPC : public Bot
{
public:
	MerchantNPC();
	MerchantNPC(uint64 id, bool attack = false);
	~MerchantNPC();

	// TODO: 근처에서 말 걸 시 상점

};


class AggressiveMerchantNPC : MerchantNPC
{
public:
	AggressiveMerchantNPC();
	AggressiveMerchantNPC(uint64 id);
	~AggressiveMerchantNPC();

	// TODO: FSM 구성하기

};