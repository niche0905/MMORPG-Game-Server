#include "pch.h"
#include "Player.h"
#include "TextureManager.h"
#include "World.h"
#include "Chat.h"


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

void Creature::Update(const int64 delta_time)
{
	for (auto it = _chattings.begin(); it != _chattings.end(); ) {
		it->Update(delta_time);
		if (not it->Validate()) {
			it = _chattings.erase(it);
		}
		else {
			++it;
		}
	}
}

void Creature::Draw(sf::RenderWindow& window)
{
	Object::Draw(window);

	if (_chattings.size() == 0) return;

	int32 chat_num = _chattings.size();
	float chat_size = 10.f;
	float chat_offset = 5.f;
	sf::Vector2f name_pos = sf::Vector2f{ static_cast<float>(position.x * TILE_SIZE + TILE_SIZE / 2), static_cast<float>(position.y * TILE_SIZE + (-TILE_SIZE / 2) - chat_offset) };
	name_pos.y -= chat_num * chat_size;
	for (auto& chat : _chattings) {
		chat.SetPosition(name_pos);
		name_pos.y += chat_size;

		chat.Draw(window);
	}
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

void Creature::AddChat(std::string_view chat)
{
	using namespace std::chrono;

	_chattings.emplace_back(chat, 2000000);
}
