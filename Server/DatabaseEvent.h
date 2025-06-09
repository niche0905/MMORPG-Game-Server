#pragma once


struct DatabaseEvent
{
	uint64	_id;
	uint8	_sp_id;	// stored procedure ID
	// TODO: 추가 정보가 필요하다면 추가하기 (권장되지 않음) clients에서 꺼내 쓰자

	DatabaseEvent();
	DatabaseEvent(uint64 id, uint8 sp_id);
};

