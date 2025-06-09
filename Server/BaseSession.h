#pragma once


class Creature
{
protected:
	uint64						_id;
	std::string					_name;

	// User와 NPC 모두 가지는 공통 Game Contents를 담는 변수
	//std::weak_ptr<ServerCore>	_server; // <- 전역 변수로 접근해도 될라나?
	bool						_is_pc;
	Position					_position;

	std::atomic<uint8>			_state = State::ST_ALLOC;

	Stats						_basic_stats = {};		// 기본 스텟(레벨에 따른 스텟)
	Stats						_temp_stats = {};		// 임시 스텟(버프, 디버프)

public:
	Creature();
	Creature(uint64 id);
	Creature(uint64 id, bool is_pc);
	Creature(uint64 id, int16 x, int16 y);
	Creature(uint64 id, int16 x, int16 y, bool is_pc);
	Creature(uint64 id, Position pos);
	Creature(uint64 id, Position pos, bool is_pc);
	virtual ~Creature() = default;

	// 필요한 메서드
	uint64 GetID() const;
	virtual bool IsPlayer() const = 0;
	virtual bool IsNPC() const = 0;

	void SetName(std::string_view name);
	std::string& GetName();
	const std::string& GetName() const;

	Position GetPosition() const;
	bool CanSee(int16 x, int16 y, int16 gap) const;
	bool CanSee(Position pos, int16 gap) const;

	uint8 GetState() const;

};

