#pragma once
#include "Types.h"


#pragma pack(push, 1)

struct BASE_PACKET
{
	uint8 _size;
	uint8 _id;

	BASE_PACKET(uint8 packet_size, uint8 packet_id)
		: _size(packet_size)
		, _id(packet_id) { }
	BASE_PACKET()
		: _size(sizeof(BASE_PACKET))
		, _id(0) { }
};

#pragma pack(pop)
