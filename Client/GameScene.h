#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "ChatUI.h"
#include "Dialog.h"

class World;
class Creature;
class ChatBox;
class Dialog;


class GameScene : public BaseScene
{
private:
	using Cooltime = std::chrono::steady_clock::time_point;

private:
	// TODO : 사용 변수들 (Pawn, Item 등등) 필요
	std::shared_ptr<World> world;
	
	std::shared_ptr<Creature> client_player;
	bool _alive = true;

	std::unordered_map<uint64, Creature> other_players;

	sf::Text player_coordinate;

	uint8 dir = DIR_LEFT;

	Cooltime	_move_cooltime;
	Cooltime	_aatk_cooltime;
	Cooltime	_satk_cooltime;
	Cooltime	_datk_cooltime;

	Stats		_stats;
	sf::Text	_stats_text;

	ChatBox		_chat_box;
	Dialog		_dialog_box;

	sf::RectangleShape	_dead_info_bg;
	sf::Text			_dead_info_text;

	uint64				_exp;
	sf::RectangleShape	_exp_bg;
	sf::RectangleShape	_exp_fg;

public:
	GameScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::vector<BYTE> packet) override;
	sf::Vector2f GetCameraCenter() const override;

private:
	// TODO : 클래스 내부에서 사용할 함수
	void StatsChangeUpdate();
	void ExpChangeUpdate();

};

