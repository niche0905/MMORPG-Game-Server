#include "pch.h"
#include "Game.h"
#include "GameScene.h"
#include "FontManager.h"


// Game ������
Game::Game()
	: window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME"}
	, view{sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)}
	, communication{}
	, my_id{ 0 }
{
	Init();
}

void Game::Init()
{
	std::wcout.imbue(std::locale("korean"));			// ǥ�� ��� ��Ʈ���� ����

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
	ConnectServer();

	while (my_id == 0) {
		ProcessPacket(communication.Recv());
	}

	while (window.isOpen()) {

		TimePoint now_time = Epoch::now();
		MicroSeconds delta_time = std::chrono::duration_cast<MicroSeconds>(now_time - last_time);
		last_time = now_time;

		HandleInput();


		window.clear();

		// Recv �ϰ� ��Ŷ ó��
		ProcessPacket(communication.Recv());

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

void Game::SendArrowKey(char dir)
{
	communication.Send(dir);
}

void Game::ProcessPacket(std::vector<char> packet)
{
	scene->ProcessPacket(packet);
}

void Game::ConnectServer()
{
	std::wcout << L"������ �ּҸ� �Է��� �ּ��� : ";
	std::string ip_address;
	std::cin >> ip_address;

	communication.Connect(ip_address.data());
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

void Game::SetID(uint64_t id)
{
	my_id = id;
}

uint64_t Game::GetID() const
{
	return my_id;
}
