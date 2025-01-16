#include "pch.h"
#include "Game.h"


// Game 생성자
Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME")
{
	// TODO : 아래의 코드가 BaseScene이 아니라 BaseScene을 상속받는 Scene이어야 함
	scene = std::make_shared<BaseScene>();
}

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

// Scene을 Load 한다
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
