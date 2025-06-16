#pragma once


struct Event
{
private:
	using time_point = std::chrono::system_clock::time_point;

public:
	enum EventType
	{
		EVENT_START = 0,

		EV_HEAL,

		EV_UPDATE,
		EV_REVIVE,

		EVENT_END
	};

public:
	uint64			_id;
	time_point		_reserved_time;
	EventType		_event_type;
	// TODO: 필요한 정보 있으면 추가하기

public:
	constexpr bool operator<(const Event& ev) const
	{
		return (_reserved_time > ev._reserved_time);
	}
	
};

