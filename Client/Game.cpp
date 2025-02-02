#include "pch.h"
#include "Game.h"
#include "GameScene.h"


// Game ������
Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME")
{
	// TODO : �Ʒ��� �ڵ尡 BaseScene�� �ƴ϶� BaseScene�� ��ӹ޴� Scene�̾�� ��
	scene = std::make_shared<BaseScene>();

	// TODO : Loading�� ������ ���� last_time�� �ʱ�ȭ �ؾ� ��
	last_time = Epoch::now();
}

// ���� ���� ���� (Input �ް�, timer ������Ʈ, Scene Update, Draw)
void Game::Run()
{
	while (true) {

		TimePoint now_time = Epoch::now();
		MicroSeconds delta_time = std::chrono::duration_cast<MicroSeconds>(now_time - last_time);

		window.clear();

		// Recv �ϰ� ��Ŷ ó��
		Update(delta_time.count());
		Draw();

		window.display();
	}
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
	case SceneType::TitleScene:
		break;
	case SceneType::GameScene:
		break;
	default:
		break;
	}
}
