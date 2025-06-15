#pragma once
#include "BaseScene.h"
#include "Player.h"

class World;
class Creature;


class GameScene : public BaseScene
{
private:
	using Cooltime = std::chrono::steady_clock::time_point;

private:
	// TODO : 사용 변수들 (Pawn, Item 등등) 필요
	std::shared_ptr<World> world;
	
	std::shared_ptr<Creature> client_player;

	std::unordered_map<uint64, Creature> other_players;

	sf::Text player_coordinate;

	uint8 dir = DIR_LEFT;

	Cooltime	_move_cooltime;
	Cooltime	_aatk_cooltime;
	Cooltime	_satk_cooltime;
	Cooltime	_datk_cooltime;

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

};

