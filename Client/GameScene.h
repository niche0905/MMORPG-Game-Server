#pragma once
#include "BaseScene.h"


class GameScene : public BaseScene
{
private:
	// TODO : ��� ������ (Pawn, Item ���) �ʿ�

public:
	void Update(const int64 delta_time) override;
	void Draw() override;
	void HandleInput(const sf::Event& event) override;

private:
	// TODO : Ŭ���� ���ο��� ����� �Լ�

};

