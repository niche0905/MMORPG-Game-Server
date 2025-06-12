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
	world = std::make_shared<World>(MAX_WIDTH, MAX_HEIGHT);

	TextureManager::Instance().LoadTexture("grass", "./Resource/Texture/grass.png");
	TileManager::Instance().AddTile(TileType::grass, "grass");

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

	client_player->Update(delta_time);
	for (auto& [c_id, creature] : other_players) {
		creature.Update(delta_time);
	}
}

// Scene에 존재하는 Object 그리기
void GameScene::Draw(sf::RenderWindow& window)
{
	auto client_position = client_player->GetPosition();
	world->Draw(window, client_position);

	client_player->Draw(window);

	for (auto& other_player : other_players) {
		other_player.second.Draw(window);
	}
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
			game.SendArrowKey(MOVE_LEFT);
		}
		if (input_event.key.code == sf::Keyboard::Right) {
			game.SendArrowKey(MOVE_RIGHT);
		}
		if (input_event.key.code == sf::Keyboard::Up) {
			game.SendArrowKey(MOVE_UP);
		}
		if (input_event.key.code == sf::Keyboard::Down) {
			game.SendArrowKey(MOVE_DOWN);
		}
		if (input_event.key.code == sf::Keyboard::A) {
			game.SendAttack(AttackType::STANDARD_ATK);

			// TODO: 아래의 입력을 통해서 attack_type 정해서 보내기 위 코드는 임시
			//		 S, D 키만 아래 해당 A는 공통으로 기본 공격
			uint8 class_type = game.GetClassType();
			switch (class_type)
			{
			case ClassType::WARRIOR:
				break;
			case ClassType::ROGUE:
				break;
			case ClassType::SORCERER:
				break;
			}
		}
	}
}

void GameScene::ProcessPacket(std::vector<BYTE> packets)
{
	size_t processed_data_size = 0;

	while (processed_data_size < packets.size())
	{
		BYTE* packet = packets.data() + processed_data_size;

		BASE_PACKET* base_packet = reinterpret_cast<BASE_PACKET*>(packet);

		switch (base_packet->_packet_id)
		{
		case PacketID::S2C_LOGIN_ALLOW:
		{
			SC_LOGIN_ALLOW_PACKET* login_packet = reinterpret_cast<SC_LOGIN_ALLOW_PACKET*>(packet);

			if (game.GetID() == 0) {
				game.SetID(static_cast<uint64>(login_packet->_id));

				client_player = std::make_shared<Creature>(world, game.GetID());
				client_player->Move(static_cast<int>(login_packet->_x), static_cast<int>(login_packet->_y));
				client_player->SetName(game.GetName());
			}
			else {
				std::cout << "re Login Allow Error\n";
				exit(-1);

			}
		}
		break;

		case PacketID::S2C_ENTER:
		{
			SC_ENTER_PACKET* enter_packet = reinterpret_cast<SC_ENTER_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(enter_packet->_id);

			other_players.try_emplace(now_id, world, now_id);
			other_players[now_id].Move(static_cast<int>(enter_packet->_x), static_cast<int>(enter_packet->_y));
			
			// TODO: 외형 정보를 이용하도록
			if (enter_packet->_id < NUM_MONSTER) {
				other_players[now_id].SetMonster();
			}
			else {
				other_players[now_id].SetDummy();
			}
			other_players[now_id].SetName(enter_packet->_name);
		}
		break;

		case PacketID::S2C_LEAVE:
		{
			SC_LEAVE_PACKET* logout_packet = reinterpret_cast<SC_LEAVE_PACKET*>(packet);

			other_players.erase(static_cast<uint64>(logout_packet->_id));
			// TODO: 또는 비활성화 처리
		}
		break;

		case PacketID::S2C_MOVE_SELF:
		{
			SC_MOVE_SELF_PACKET* move_update_packet = reinterpret_cast<SC_MOVE_SELF_PACKET*>(packet);

			client_player->Move(static_cast<int>(move_update_packet->_x), static_cast<int>(move_update_packet->_y));
		}
		break;

		case PacketID::S2C_MOVE:
		{
			SC_MOVE_PACKET* move_packet = reinterpret_cast<SC_MOVE_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(move_packet->_id);
			other_players[now_id].Move(static_cast<int>(move_packet->_x), static_cast<int>(move_packet->_y));
		}
		break;

		case PacketID::S2C_CHAT:
		{
			SC_CHAT_PACKET* chat_packet = reinterpret_cast<SC_CHAT_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(chat_packet->_id);
			other_players[now_id].AddChat(chat_packet->_message);
		}
		break;

		default:
			std::cout << "패킷 id 없는 패킷 오류\n";
			break;
		}

		processed_data_size += base_packet->_size;
	}
}

sf::Vector2f GameScene::GetCameraCenter() const
{
	sf::Vector2i player_position = client_player->GetPosition();
	float offset = client_player->GetOffset();;
	sf::Vector2f camera_center = sf::Vector2f(player_position.x * TILE_SIZE + offset, player_position.y * TILE_SIZE + offset);
	return camera_center;
}
