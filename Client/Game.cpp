#include "pch.h"
#include "Game.h"


// Game ������
Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME")
{
	// TODO : �Ʒ��� �ڵ尡 BaseScene�� �ƴ϶� BaseScene�� ��ӹ޴� Scene�̾�� ��
	scene = std::make_shared<BaseScene>();
}

// ���� ���� ���� (Input �ް�, timer ������Ʈ, Scene Update, Draw)
void Game::Run()
{

}

// ���� ��� ������Ʈ (Scene ������Ʈ)
void Game::Update(const int64 delta_time)
{

}

// ���� ��� ������ (Scene Draw)
void Game::Draw()
{

}

// Input �ޱ� (�޾Ƽ� Scene���� �ѱ��)
void Game::HandleInput()
{

}

// Scene�� Load �Ѵ�
void Game::SceneLoad(SceneType scene_type)
{
	switch (scene_type)
	{
	case TitleScene:
		break;
	case GameScene:
		break;
	default:
		break;
	}
}
