#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "World.h"


Creature::Creature()
	: Object{}
	, id{ 0 }
{
	Init();
}

Creature::Creature(std::shared_ptr<World> world)
	: Object{ world }
	, id{ 0 }
{
	Init();
}

Creature::Creature(std::shared_ptr<World> world, uint64 id)
	: Object{ world }
	, id{ id }
{
	Init();
}

void Creature::Init()
{
	SetSize(PLAYER_SIZE);
	TextureManager::Instance().LoadTexture("player", "./Resource/Texture/player.png");
	SetSprite("player");
}

uint64 Creature::GetID() const
{
	return id;
}

void Creature::SetDummy()
{
	TextureManager::Instance().LoadTexture("dummy", "./Resource/Texture/dummy.png");
	SetSprite("dummy");
}

void Creature::SetMonster()
{
	TextureManager::Instance().LoadTexture("monster", "./Resource/Texture/monster.png");
	SetSprite("monster");
}
