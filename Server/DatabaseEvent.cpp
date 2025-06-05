#include "pch.h"
#include "DatabaseEvent.h"


DatabaseEvent::DatabaseEvent()
	: DatabaseEvent{ -1, 0 }
{

}

DatabaseEvent::DatabaseEvent(int64 id, uint8 sp_id)
	: _id{ id }
	, _sp_id{ sp_id }
{

}
