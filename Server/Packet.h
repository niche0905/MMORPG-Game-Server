#pragma once

#include <cstdint>


namespace myNP		//	my Network Protocol
{
	enum class PacketID : uint8_t
	{
		CS_KEY_INPUT = 1,
		SC_LOGIN_USER,
		SC_LOGOUT_USER,
		SC_MOVE_USER,

		END
	};


#pragma pack(push, 1)

	struct BASE_PACKET
	{
		uint8_t _size;
		PacketID _id;

		BASE_PACKET(uint8_t packet_size, PacketID packet_id) 
			: _size(packet_size)
			, _id(packet_id) {}
		BASE_PACKET() 
			: _size(2)
			, _id(static_cast<PacketID>(0)) {}
	};

	struct CS_KEY_INPUT : public BASE_PACKET
	{
		uint8_t _user_id;
		uint8_t _direction;

		CS_KEY_INPUT(uint8_t user_id, uint8_t direction)
			: BASE_PACKET(sizeof(CS_KEY_INPUT), PacketID::CS_KEY_INPUT)
			, _user_id{ user_id }
			, _direction{ direction } {}
	};

	struct SC_LOGIN_USER : public BASE_PACKET
	{
		uint8_t _user_id;
		uint8_t _x;
		uint8_t _y;

		SC_LOGIN_USER(uint8_t user_id, uint8_t x, uint8_t y)
			: BASE_PACKET(sizeof(SC_LOGIN_USER), PacketID::SC_LOGIN_USER)
			, _user_id{ user_id }
			, _x{ x }
			, _y{ y } {}
	};

	struct SC_LOGOUT_USER : public BASE_PACKET
	{
		uint8_t _user_id;

		SC_LOGOUT_USER(uint8_t user_id)
			: BASE_PACKET(sizeof(SC_LOGOUT_USER), PacketID::SC_LOGOUT_USER)
			, _user_id{ user_id } {}
	};

	struct SC_MOVE_USER : public BASE_PACKET
	{
		uint8_t _user_id;
		uint8_t _x;
		uint8_t _y;

		SC_MOVE_USER(uint8_t user_id, uint8_t x, uint8_t y)
			: BASE_PACKET(sizeof(SC_MOVE_USER), PacketID::SC_MOVE_USER)
			, _user_id{ user_id }
			, _x{ x }
			, _y{ y } {}
	};

#pragma pack(pop)

}
