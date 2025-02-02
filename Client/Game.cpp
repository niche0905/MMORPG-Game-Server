#include "pch.h"
#include "Game.h"
#include "GameScene.h"


// Game 생성자
Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME")
{
	// TODO : 아래의 코드가 BaseScene이 아니라 BaseScene을 상속받는 Scene이어야 함
	scene = std::make_shared<GameScene>();

	// TODO : Loading이 끝나고 나서 last_time을 초기화 해야 함
	last_time = Epoch::now();
}

// 게임 실행 루프 (Input 받고, timer 업데이트, Scene Update, Draw)
void Game::Run()
{
	while (window.isOpen()) {

		TimePoint now_time = Epoch::now();
		MicroSeconds delta_time = std::chrono::duration_cast<MicroSeconds>(now_time - last_time);

		HandleInput();

		window.clear();

		// Recv 하고 패킷 처리
		Update(delta_time.count());
		Draw();

		window.display();
	}
}

// 게임 요소 업데이트 (Scene 업데이트)
void Game::Update(const int64 delta_time)
{
	scene->Update(delta_time);
}

// 게임 요소 렌더링 (Scene Draw)
void Game::Draw()
{
	scene->Draw(window);
}

// Input 받기 (받아서 Scene으로 넘기기)
void Game::HandleInput()
{
	sf::Event input_event;

	while (window.pollEvent(input_event)) {

		// TODO : 모든 상황에서 필요한 입력 처리
		if (input_event.type == sf::Event::Closed) {
			window.close();
		}

		scene->HandleInput(input_event);
	}
}

// Scene을 Load 한다
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
