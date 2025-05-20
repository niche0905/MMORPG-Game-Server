#pragma once


class Creature
{
protected:
	int64		_id;

	// User�� NPC ��� ������ ���� Game Contents�� ��� ����
	Position	_position;

	Stats		_basic_stats = {};		// �⺻ ����(������ ���� ����)
	Stats		_temp_stats = {};		// �ӽ� ����(����, �����)

public:
	Creature();
	Creature(int64 id, int16 x, int16 y);
	Creature(int64 id, Position pos);
	~Creature();

	// �ʿ��� �޼���
	int64 GetID() const;
	virtual bool IsPlayer() const = 0;
	virtual bool IsNPC() const = 0;

};

