#pragma once


struct BaseStats			// ��� ĳ����/��� ������ ����
{
	uint16 HP	= 0;		// ü��
	uint16 ATK	= 0;		// ���ݷ�
	uint16 DEF	= 0;		// ����
	uint16 STR	= 0;		// ��
	uint16 INT	= 0;		// ����
	uint16 DEX	= 0;		// �ؾ�
	uint16 CRT	= 0;		// ġ��Ÿ
	uint16 MOV	= 0;		// �̵���

	BaseStats();
	BaseStats(uint16 hp, uint16 atk, uint16 def, uint16 str, uint16 intelli, uint16 dex, uint16 crt, uint16 mov);

	BaseStats operator+(const BaseStats& other) const;
	BaseStats operator+=(const BaseStats& other);
};

struct CharacterStats	
{
	uint8	level = 1;	// ĳ���Ͱ� ������ ���� (�� �� �־�� ��)
};

struct UserStats : CharacterStats
{
	uint32	exp = 0;	// �÷��̾ ������ ����ġ
};

using Stats = BaseStats;
