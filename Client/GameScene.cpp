#include "pch.h"
#include "GameScene.h"


void GameScene::Init()
{
	shape.setSize(sf::Vector2f(50, 50));
	shape.setOrigin(25, 50);
	shape.setPosition(0, 0);
	shape.setFillColor(sf::Color::Green);
}

// Scene�� �����ϴ� Object ������Ʈ
void GameScene::Update(const int64 delta_time)
{

}

// Scene�� �����ϴ� Object �׸���
void GameScene::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

// Player���� Input �����ϱ�
void GameScene::HandleInput(const sf::Event& event)
{

}
