#include "pch.h"
#include "MerchantNPC.h"


MerchantNPC::MerchantNPC()
	: MerchantNPC{ INVALID_ID }
{

}

MerchantNPC::MerchantNPC(uint64 id, bool attack, uint8 b_type)
	: Bot{ id, b_type, false, true, attack, not attack, attack }
{

}

MerchantNPC::~MerchantNPC()
{

}



AggressiveMerchantNPC::AggressiveMerchantNPC()
	: AggressiveMerchantNPC{ INVALID_ID }
{

}

AggressiveMerchantNPC::AggressiveMerchantNPC(uint64 id)
	: MerchantNPC{ id, true, ClassType::AGRS_MERCHANT_NPC }
{

}

AggressiveMerchantNPC::~AggressiveMerchantNPC()
{

}
