#pragma once
#include "BaseScene.h"

class World;
class Player;


class GameScene : public BaseScene
{
private:
	// TODO : ��� ������ (Pawn, Item ���) �ʿ�
	std::shared_ptr<World> world;
	
	std::shared_ptr<Player> client_player;

	sf::Text player_coordinate;

public:
	GameScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HUD(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;
	void ProcessPacket(std::string packet) override;
	sf::Vector2f GetCameraCenter() const override;

private:
	// TODO : Ŭ���� ���ο��� ����� �Լ�

};

