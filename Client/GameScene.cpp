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
	world = std::make_shared<World>();
	world->LoadWorld();

	TileManager::Instance().AddTile(TileType::grass, "grass");
	TileManager::Instance().AddTile(TileType::water, "water");
	TileManager::Instance().AddTile(TileType::house, "house");
	TileManager::Instance().AddTile(TileType::tree, "tree");

	player_coordinate.setFont(FontManager::Instance().GetFont("neodot"));
	player_coordinate.setCharacterSize(20);
	player_coordinate.setFillColor(sf::Color::White);
	player_coordinate.setStyle(sf::Text::Regular);
	player_coordinate.setPosition(10.0f, 10.0f);
	player_coordinate.setOutlineColor(sf::Color::Black);
	player_coordinate.setOutlineThickness(1.0f);

	using namespace std::chrono;

	_move_cooltime = steady_clock::now() - MOVE_COOLTIME;
	_aatk_cooltime = steady_clock::now() - AATK_COOLTIME;
	_satk_cooltime = steady_clock::now() - SATK_COOLTIME;
	_datk_cooltime = steady_clock::now() - DATK_COOLTIME;
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

	for (auto& other_player : other_players) {
		other_player.second.Draw(window);
	}

	client_player->Draw(window);
}

void GameScene::HUD(sf::RenderWindow& window)
{
	window.draw(player_coordinate);

	_chat_box.Draw(window);
}

// Player에게 Input 전달하기
void GameScene::HandleInput(const sf::Event& input_event)
{
	using namespace std::chrono;
	auto now_time = steady_clock::now();

	_chat_box.HandleInput(input_event);

	if (input_event.type == sf::Event::KeyPressed) {
		if (_alive) {

			if (input_event.key.code == sf::Keyboard::Left) {
				dir = DIR_LEFT;
				if (game.IsDev());
				else {
					if ((now_time - _move_cooltime) < (MOVE_COOLTIME)) {
						return;		// 이동 실패
					}
				}
				_move_cooltime = now_time;
				game.SendArrowKey(MOVE_LEFT);
			}
			if (input_event.key.code == sf::Keyboard::Right) {
				dir = DIR_RIGHT;
				if (game.IsDev());
				else {
					if ((now_time - _move_cooltime) < (MOVE_COOLTIME)) {
						return;		// 이동 실패
					}
				}
				_move_cooltime = now_time;
				game.SendArrowKey(MOVE_RIGHT);
			}
			if (input_event.key.code == sf::Keyboard::Up) {
				dir = DIR_UP;
				if (game.IsDev());
				else {
					if ((now_time - _move_cooltime) < (MOVE_COOLTIME)) {
						return;		// 이동 실패
					}
				}
				_move_cooltime = now_time;
				game.SendArrowKey(MOVE_UP);
			}
			if (input_event.key.code == sf::Keyboard::Down) {
				dir = DIR_DOWN;
				if (game.IsDev());
				else {
					if ((now_time - _move_cooltime) < (MOVE_COOLTIME)) {
						return;		// 이동 실패
					}
				}
				_move_cooltime = now_time;
				game.SendArrowKey(MOVE_DOWN);
			}
			if (input_event.key.code == sf::Keyboard::A) {
				if (game.IsDev());
				else {
					if ((now_time - _aatk_cooltime) < (AATK_COOLTIME)) {
						return;		// A공격 실패
					}
				}
				_aatk_cooltime = now_time;
				game.SendAttack(KeyType::KEY_A, dir);
			}
			if (input_event.key.code == sf::Keyboard::S) {
				if (game.IsDev());
				else {
					if ((now_time - _satk_cooltime) < (SATK_COOLTIME)) {
						return;		// S공격 실패
					}
				}
				_satk_cooltime = now_time;
				game.SendAttack(KeyType::KEY_S, dir);
			}
			if (input_event.key.code == sf::Keyboard::D) {
				if (game.IsDev());
				else {
					if ((now_time - _datk_cooltime) < (DATK_COOLTIME)) {
						return;		// D공격 실패
					}
				}
				_datk_cooltime = now_time;
				game.SendAttack(KeyType::KEY_D, dir);
			}
			if (input_event.key.code == sf::Keyboard::Numpad4) {
				dir = DIR_LEFT;
			}
			if (input_event.key.code == sf::Keyboard::Numpad6) {
				dir = DIR_RIGHT;
			}
			if (input_event.key.code == sf::Keyboard::Numpad8) {
				dir = DIR_UP;
			}
			if (input_event.key.code == sf::Keyboard::Numpad2) {
				dir = DIR_DOWN;
			}
		}
		else {

			if (input_event.key.code == sf::Keyboard::R) {
				// TODO: 부활을 요청하는 패킷
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
				game.SetID(login_packet->_id);

				client_player = std::make_shared<Creature>(world, game.GetID());
				client_player->Move(static_cast<int>(login_packet->_x), static_cast<int>(login_packet->_y));
				client_player->SetVisualInfo(login_packet->_visual_info);
				client_player->SetName(game.GetName());
				client_player->SetClassType(login_packet->_class_type);
				client_player->SetMaxHP(login_packet->_max_hp);
				client_player->ChangeHP(login_packet->_hp);
				client_player->SetDamageColor(sf::Color{ 145, 101, 226 });
			}
			else {
				std::cout << "re Login Allow Error\n";
				exit(-1);

			}
		}
		break;

		case PacketID::S2C_DAMAGE:
		{
			SC_DAMAGE_PACKET* damage_packet = reinterpret_cast<SC_DAMAGE_PACKET*>(packet);

			for (int index = 0; index < damage_packet->_num; ++index) {

				DamageInfo& damage_info = damage_packet->_damage_infos[index];
				other_players[damage_info._id].AddDamage(damage_info._damage);
			}
		}
		break;

		case PacketID::S2C_DEAD:
		{
			SC_DEAD_PACKET* damage_packet = reinterpret_cast<SC_DEAD_PACKET*>(packet);

			_alive = false;
		}
		break;

		case PacketID::S2C_REVIVE:
		{
			SC_REVIVE_PACKET* damage_packet = reinterpret_cast<SC_REVIVE_PACKET*>(packet);

			_alive = true;
		}
		break;

		case PacketID::S2C_ENTER:
		{
			SC_ENTER_PACKET* enter_packet = reinterpret_cast<SC_ENTER_PACKET*>(packet);

			uint64 now_id = enter_packet->_id;

			other_players.try_emplace(now_id, world, now_id);
			other_players[now_id].Move(static_cast<int>(enter_packet->_x), static_cast<int>(enter_packet->_y));
			other_players[now_id].SetVisualInfo(enter_packet->_visual_info);
			other_players[now_id].SetName(enter_packet->_name);
			other_players[now_id].SetClassType(enter_packet->_class_type);
			other_players[now_id].SetMaxHP(enter_packet->_max_hp);
			other_players[now_id].ChangeHP(enter_packet->_hp);
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

		case PacketID::S2C_ATTACK:
		{
			SC_ATTACK_PACKET* atk_packet = reinterpret_cast<SC_ATTACK_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(atk_packet->_id);

			if (now_id == game.GetID()) {
				client_player->ShowAttack(atk_packet->_atk_key, atk_packet->_atk_dir);
			}
			else {
				other_players[now_id].ShowAttack(atk_packet->_atk_key, atk_packet->_atk_dir);
			}
		}
		break;

		case PacketID::S2C_UPDATE_VI:
		{
			SC_UPDATE_VI_PACKET* update_vi_packet = reinterpret_cast<SC_UPDATE_VI_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(update_vi_packet->_id);
			
			if (now_id == game.GetID()) {
				client_player->SetVisualInfo(update_vi_packet->_vi);
			}
			else {
				other_players[now_id].SetVisualInfo(update_vi_packet->_vi);
			}
		}
		break;

		case PacketID::S2C_HP_CHANGE:
		{
			SC_HP_CHANGE_PACKET* hp_update_packet = reinterpret_cast<SC_HP_CHANGE_PACKET*>(packet);

			uint64 now_id = static_cast<uint64>(hp_update_packet->_id);

			if (now_id == game.GetID()) {
				uint16 hp_gap = client_player->GetHP() - hp_update_packet->_hp;
				client_player->AddDamage(hp_gap);
				client_player->ChangeHP(hp_update_packet->_hp);
			}
			else {
				other_players[now_id].ChangeHP(hp_update_packet->_hp);
			}
		}
		break;

		default:
			std::cout << "Packet id Done Exist\n";
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
