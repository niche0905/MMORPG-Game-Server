#include "pch.h"
#include "MerchantNPC.h"


MerchantNPC::MerchantNPC()
	: MerchantNPC{ INVALID_ID }
{

}

MerchantNPC::MerchantNPC(uint64 id, bool attack)
	: Bot{ id, false, true, attack, attack }
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
	: MerchantNPC{ id, true }
{

}

AggressiveMerchantNPC::~AggressiveMerchantNPC()
{

}
