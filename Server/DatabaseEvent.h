#pragma once


struct DatabaseEvent
{
	enum DbOperation : uint8
	{
		DB_START = 0,

		// TODO: 필요한 DB OP 추가하기
		//		 EX) DB LOGIN 등
		DB_LOGIN_REQUEST,
		DB_REGISTER_REQUEST,

		DB_END
	};

	uint64	_id;
	uint8	_sp_id;	// stored procedure ID
	// TODO: 추가 정보가 필요하다면 추가하기 (권장되지 않음) clients에서 꺼내 쓰자

	DatabaseEvent();
	DatabaseEvent(uint64 id, uint8 sp_id);
};

