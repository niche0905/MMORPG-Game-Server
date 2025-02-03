#pragma once
#include "BaseScene.h"
#include "World.h"

class World;


class GameScene : public BaseScene
{
private:
	// TODO : ��� ������ (Pawn, Item ���) �ʿ�
	World world;

public:
	GameScene();

	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& event) override;

private:
	// TODO : Ŭ���� ���ο��� ����� �Լ�

};

