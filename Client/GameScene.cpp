#include "pch.h"
#include "GameScene.h"


void GameScene::Init()
{
	// TEMP : Draw �׽�Ʈ������ �ӽ� shape
	shape.setSize(sf::Vector2f(50, 50));
	shape.setOrigin(25, 50);
	shape.setPosition(50, 50);
	shape.setFillColor(sf::Color::Green);
}

// Scene�� �����ϴ� Object ������Ʈ
void GameScene::Update(const int64 delta_time)
{

}

// Scene�� �����ϴ� Object �׸���
void GameScene::Draw(sf::RenderWindow& window)
{
	// TEMP : Draw �׽�Ʈ������ �ӽ� Init ��ġ
	Init();
	// TEMP : Draw �׽�Ʈ������ �ӽ� shape
	window.draw(shape);
}

// Player���� Input �����ϱ�
void GameScene::HandleInput(const sf::Event& event)
{

}
