#include "pch.h"
#include "Game.h"
#include "GameScene.h"
#include "FontManager.h"


// Game ������
Game::Game()
	: window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME"}
	, view{sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)}
	, communication{}
{
	Init();
}

void Game::Init()
{
	if (not FontManager::Instance().LoadFont("neodot", "Resource/Font/neodgm.ttf")) {
		std::wcout << L"��Ʈ �ε� ����\n";

		exit(-1);
	}

	scene = std::make_shared<GameScene>();

	// TODO : Loading�� ������ ���� last_time�� �ʱ�ȭ �ؾ� ��
	last_time = Epoch::now();
}

// ���� ���� ���� (Input �ް�, timer ������Ʈ, Scene Update, Draw)
void Game::Run()
{
	while (window.isOpen()) {

		TimePoint now_time = Epoch::now();
		MicroSeconds delta_time = std::chrono::duration_cast<MicroSeconds>(now_time - last_time);
		last_time = now_time;

		HandleInput();

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
	scene->Update(delta_time);
}

// ���� ��� ������ (Scene Draw)
void Game::Draw()
{
	SetCameraView();
	scene->Draw(window);

	window.setView(window.getDefaultView());
	scene->HUD(window);
}

// Input �ޱ� (�޾Ƽ� Scene���� �ѱ��)
void Game::HandleInput()
{
	sf::Event input_event;

	while (window.pollEvent(input_event)) {

		// TODO : ��� ��Ȳ���� �ʿ��� �Է� ó��
		if (input_event.type == sf::Event::Closed) {
			window.close();
		}

		if (scene)
			scene->HandleInput(input_event);
	}
}

void Game::SetCameraView()
{
	view.setCenter(scene->GetCameraCenter());
	window.setView(view);
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
