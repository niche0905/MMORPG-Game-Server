#pragma once
#include "BaseScene.h"
#include "Player.h"

class World;
class Player;


class GameScene : public BaseScene
{
private:
	// TODO : ��� ������ (Pawn, Item ���) �ʿ�
	std::shared_ptr<World> world;
	
	std::shared_ptr<Player> client_player;

	std::unordered_map<uint64_t, Player> other_players;

	sf::Text player_coordinate;

public:
	GameScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::vector<char> packet) override;
	sf::Vector2f GetCameraCenter() const override;

private:
	// TODO : Ŭ���� ���ο��� ����� �Լ�

};

