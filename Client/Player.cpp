#include "pch.h"
#include "Player.h"
#include "TextureManager.h"


Player::Player()
	: Object{}
{

}

void Player::Init()
{
	SetSize(player_size);
	TextureManager::Instance().LoadTexture("player", "./Resource/Texture/player.png");
	SetSprite("player");
}
