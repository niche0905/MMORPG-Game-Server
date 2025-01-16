#include "pch.h"
#include "Game.h"


// 게임 실행 루프 (Input 받고, timer 업데이트, Scene Update, Draw)
void Game::Run()
{

}

// 게임 요소 업데이트 (Scene 업데이트)
void Game::Update(const int64 delta_time)
{

}

// 게임 요소 렌더링 (Scene Draw)
void Game::Draw()
{

}

// Input 받기 (받아서 Scene으로 넘기기)
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
