#pragma once
#include "BaseScene.h"

class World;
class Player;


class GameScene : public BaseScene
{
private:
	// TODO : 사용 변수들 (Pawn, Item 등등) 필요
	std::shared_ptr<World> world;
	
	std::shared_ptr<Player> client_player;

public:
	GameScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& input_event) override;

private:
	// TODO : 클래스 내부에서 사용할 함수

};

