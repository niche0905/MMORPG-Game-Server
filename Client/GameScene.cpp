#include "pch.h"
#include "GameScene.h"
#include "TileManager.h"
#include "TextureManager.h"


GameScene::GameScene()
	: world{ 100, 100 }
{
	Init();
}

void GameScene::Init()
{
	TextureManager::Instance().LoadTexture("grass", "./Resource/Texture/TestGrass.jpg");
	TileManager::Instance().Add(TileType::grass, "grass");
}

// Scene�� �����ϴ� Object ������Ʈ
void GameScene::Update(const int64 delta_time)
{

}

// Scene�� �����ϴ� Object �׸���
void GameScene::Draw(sf::RenderWindow& window)
{
	world.Draw(window);
}

// Player���� Input �����ϱ�
void GameScene::HandleInput(const sf::Event& event)
{

}
