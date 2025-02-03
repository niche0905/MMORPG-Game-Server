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

	client_player = std::make_shared<Player>(world);
}

// Scene에 존재하는 Object 업데이트
void GameScene::Update(const int64 delta_time)
{

}

// Scene에 존재하는 Object 그리기
void GameScene::Draw(sf::RenderWindow& window)
{
	world->Draw(window);

	client_player->Draw(window);
}

// Player에게 Input 전달하기
void GameScene::HandleInput(const sf::Event& input_event)
{
	if (input_event.type == sf::Event::KeyPressed) {
		if (input_event.key.code == sf::Keyboard::Left) {
			client_player->Shift(-1, 0);
		}
		if (input_event.key.code == sf::Keyboard::Right) {
			client_player->Shift(+1, 0);
		}
		if (input_event.key.code == sf::Keyboard::Up) {
			client_player->Shift(0, -1);
		}
		if (input_event.key.code == sf::Keyboard::Down) {
			client_player->Shift(0, +1);
		}


	}
}
