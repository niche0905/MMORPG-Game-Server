#pragma once


class MerchantNPC : public Bot
{
public:
	MerchantNPC();
	MerchantNPC(uint64 id, bool attack = false);
	~MerchantNPC();

};


class AggressiveMerchantNPC : MerchantNPC
{
public:
	AggressiveMerchantNPC();
	AggressiveMerchantNPC(uint64 id);
	~AggressiveMerchantNPC();

};