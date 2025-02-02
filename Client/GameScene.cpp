#include "pch.h"
#include "GameScene.h"


void GameScene::Init()
{
	shape.setSize(sf::Vector2f(50, 50));
	shape.setOrigin(25, 50);
	shape.setPosition(0, 0);
	shape.setFillColor(sf::Color::Green);
}

// Scene에 존재하는 Object 업데이트
void GameScene::Update(const int64 delta_time)
{

}

// Scene에 존재하는 Object 그리기
void GameScene::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

// Player에게 Input 전달하기
void GameScene::HandleInput(const sf::Event& event)
{

}
