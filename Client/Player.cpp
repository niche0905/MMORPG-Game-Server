#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "World.h"


Player::Player()
	: Object{}
{
	Init();
}

Player::Player(std::shared_ptr<World> world)
	: Object{ world }
{
	Init();
}

void Player::Init()
{
	SetSize(player_size);
	TextureManager::Instance().LoadTexture("player", "./Resource/Texture/player.png");
	SetSprite("player");
}
