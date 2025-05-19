#pragma once


class BaseSession
{
protected:
	int64		_id;

	// User�� NPC ��� ������ ���� Game Contents�� ��� ����
	Stats		_basic_stats;
	Stats		_temp_stats;

public:
	BaseSession();
	BaseSession(int64 id);
	~BaseSession();

	// �ʿ��� �޼���
	int64 GetID() const;
	virtual bool IsPlayer() const = 0;
	virtual bool IsNPC() const = 0;

};

