#pragma once
#include "BaseScene.h"


class GameScene : public BaseScene
{
private:
	// TODO : ��� ������ (Pawn, Item ���) �ʿ�
	sf::RectangleShape shape;

public:
	void Init();

	void Update(const int64 delta_time) override;
	void Draw(sf::RenderWindow& window) override;
	void HandleInput(const sf::Event& event) override;

private:
	// TODO : Ŭ���� ���ο��� ����� �Լ�

};

