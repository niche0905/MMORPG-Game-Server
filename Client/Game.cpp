#include "pch.h"
#include "Game.h"


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
