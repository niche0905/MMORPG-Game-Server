#include "pch.h"
#include "DatabaseEvent.h"


DatabaseEvent::DatabaseEvent()
	: DatabaseEvent{ INVALID_ID, 0 }
{

}

DatabaseEvent::DatabaseEvent(uint64 id, uint8 sp_id)
	: _id{ id }
	, _sp_id{ sp_id }
{

}
