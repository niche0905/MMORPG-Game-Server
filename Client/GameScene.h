#pragma once
#include "BaseScene.h"


class GameScene : public BaseScene
{
private:
	// TODO : 사용 변수들 (Pawn, Item 등등) 필요

public:
	void Update(const int64 delta_time) override;
	void Draw() override;
	void HandleInput(const sf::Event& event) override;

private:
	// TODO : 클래스 내부에서 사용할 함수

};

