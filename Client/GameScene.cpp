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

// Scene에 존재하는 Object 업데이트
void GameScene::Update(const int64 delta_time)
{

}

// Scene에 존재하는 Object 그리기
void GameScene::Draw(sf::RenderWindow& window)
{
	world.Draw(window);
}

// Player에게 Input 전달하기
void GameScene::HandleInput(const sf::Event& event)
{

}
