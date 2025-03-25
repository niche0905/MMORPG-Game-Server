#include "pch.h"
#include "GameScene.h"
#include "TileManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Game.h"
#include "World.h"
#include "Player.h"


GameScene::GameScene()
{
	Init();
}

void GameScene::Init()
{
	world = std::make_shared<World>(8, 8);

	TextureManager::Instance().LoadTexture("grass", "./Resource/Texture/grass.png");
	TileManager::Instance().AddTile(TileType::grass, "grass");

	client_player = std::make_shared<Player>(world);

	player_coordinate.setFont(FontManager::Instance().GetFont("neodot"));
	player_coordinate.setCharacterSize(20);
	player_coordinate.setFillColor(sf::Color::White);
	player_coordinate.setStyle(sf::Text::Regular);
	player_coordinate.setPosition(10.0f, 10.0f);
	player_coordinate.setOutlineColor(sf::Color::Black);
	player_coordinate.setOutlineThickness(1.0f);
}

// Scene에 존재하는 Object 업데이트
void GameScene::Update(const int64 delta_time)
{
	sf::Vector2i player_position = client_player->GetPosition();

	std::string position_str = "(" + std::to_string(player_position.x) + ", " + std::to_string(player_position.y) + ")";
	player_coordinate.setString(position_str);
}

// Scene에 존재하는 Object 그리기
void GameScene::Draw(sf::RenderWindow& window)
{
	world->Draw(window);

	client_player->Draw(window);
}

void GameScene::HUD(sf::RenderWindow& window)
{
	window.draw(player_coordinate);
}

// Player에게 Input 전달하기
void GameScene::HandleInput(const sf::Event& input_event)
{
	if (input_event.type == sf::Event::KeyPressed) {
		if (input_event.key.code == sf::Keyboard::Left) {
			//client_player->Shift(-1, 0);
			game.SendArrowKey(1);
		}
		if (input_event.key.code == sf::Keyboard::Right) {
			//client_player->Shift(+1, 0);
			game.SendArrowKey(3);
		}
		if (input_event.key.code == sf::Keyboard::Up) {
			//client_player->Shift(0, -1);
			game.SendArrowKey(0);
		}
		if (input_event.key.code == sf::Keyboard::Down) {
			//client_player->Shift(0, +1);
			game.SendArrowKey(2);
		}


	}
}

void GameScene::ProcessPacket(std::string packet)
{
	char x{ packet[0] }, y{ packet[1] };
	client_player->Move(x, y);
}

sf::Vector2f GameScene::GetCameraCenter() const
{
	sf::Vector2i player_position = client_player->GetPosition();
	float offset = client_player->GetOffset();;
	sf::Vector2f camera_center = sf::Vector2f(player_position.x * tile_size + offset, player_position.y * tile_size + offset);
	return camera_center;
}
