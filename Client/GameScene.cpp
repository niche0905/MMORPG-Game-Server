#include "pch.h"
#include "GameScene.h"
#include "TileManager.h"
#include "TextureManager.h"
#include "World.h"
#include "Player.h"


GameScene::GameScene()
{
	Init();
}

void GameScene::Init()
{
	world = std::make_shared<World>(100, 100);

	TextureManager::Instance().LoadTexture("grass", "./Resource/Texture/grass.png");
	TileManager::Instance().Add(TileType::grass, "grass");

	client_player = std::make_shared<Player>();
}

// Scene�� �����ϴ� Object ������Ʈ
void GameScene::Update(const int64 delta_time)
{

}

// Scene�� �����ϴ� Object �׸���
void GameScene::Draw(sf::RenderWindow& window)
{
	world->Draw(window);

	client_player->Draw(window);
}

// Player���� Input �����ϱ�
void GameScene::HandleInput(const sf::Event& event)
{

}
