#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "World.h"


Player::Player()
	: Object{}
	, id{ 0 }
{
	Init();
}

Player::Player(std::shared_ptr<World> world)
	: Object{ world }
	, id{ 0 }
{
	Init();
}

Player::Player(std::shared_ptr<World> world, uint64_t id)
	: Object{ world }
	, id{ id }
{
	Init();
}

void Player::Init()
{
	SetSize(player_size);
	TextureManager::Instance().LoadTexture("player", "./Resource/Texture/player.png");
	SetSprite("player");
}

uint64_t Player::GetID() const
{
	return id;
}

void Player::SetDummy()
{
	TextureManager::Instance().LoadTexture("dummy", "./Resource/Texture/dummy.png");
	SetSprite("dummy");
}
