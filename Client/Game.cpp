#include "pch.h"
#include "Game.h"
#include "GameScene.h"
#include "FontManager.h"


// Game 생성자
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
	std::wcout.imbue(std::locale("korean"));			// 표준 출력 스트림에 적용

	if (not FontManager::Instance().LoadFont("neodot", "Resource/Font/neodgm.ttf")) {
		std::wcout << L"폰트 로드 실패\n";

		exit(-1);
	}

	scene = std::make_shared<GameScene>();

	// TODO : Loading이 끝나고 나서 last_time을 초기화 해야 함
	last_time = Epoch::now();
}

// 게임 실행 루프 (Input 받고, timer 업데이트, Scene Update, Draw)
void Game::Run()
{
	ConnectServer();

	AttemptLogin();

	while (my_id == 0) {
		ProcessPacket(communication.Recv());
	}

	std::cout << "Login Done\n";

	while (window.isOpen()) {

		TimePoint now_time = Epoch::now();
		MicroSeconds delta_time = std::chrono::duration_cast<MicroSeconds>(now_time - last_time);
		last_time = now_time;

		HandleInput();

		window.clear();

		// Recv 하고 패킷 처리
		ProcessPacket(communication.Recv());

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
	SetCameraView();
	scene->Draw(window);

	window.setView(window.getDefaultView());
	scene->HUD(window);
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

		if (scene)
			scene->HandleInput(input_event);
	}
}

void Game::SendArrowKey(uint8 dir)
{
	// TODO: move_time 적용하기
	CS_MOVE_PACKET move_packet{ dir, 0 };
	communication.Send(reinterpret_cast<BYTE*>(&move_packet), sizeof(move_packet));
}

void Game::ProcessPacket(std::vector<BYTE> packet)
{
	if (packet.size() == 0)
		return;

	scene->ProcessPacket(packet);
}

void Game::ConnectServer()
{
#ifdef LOOPBACK
	std::string temp;
	std::wcout << L"루프팩 접속 대기 : ";
	std::cin >> temp;
	std::wcout << L"루프백 접속 중...\n";
	std::string ip_address = LOOPBACK_ADDRESS;
#else
	std::wcout << L"접속할 주소를 입력해 주세요 : ";
	std::string ip_address;
	std::cin >> ip_address;
#endif

	communication.Connect(ip_address.data());
}

void Game::AttemptLogin()
{
	std::wcout << L"사용할 닉네임을 입력해 주세요 : ";
	std::cin >> name;

	CS_LOGIN_PACKET login_packet(name.data());
	communication.Send(reinterpret_cast<BYTE*>(&login_packet), sizeof(login_packet));
}

void Game::SetCameraView()
{
	view.setCenter(scene->GetCameraCenter());
	window.setView(view);
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

void Game::SetID(uint64 id)
{
	my_id = id;
}

uint64 Game::GetID() const
{
	return my_id;
}

const std::string& Game::GetName() const
{
	return name;
}
