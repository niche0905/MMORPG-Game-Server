#include "pch.h"
#include "Game.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "LoginScene.h"
#include "CreateScene.h"
#include "FontManager.h"
#include "TextureManager.h"


// Game 생성자
Game::Game()
	: window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "MMORPG GAME"}
	, view{sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)}
	, communication{}
	, my_id{ 0 }
{
	using namespace std::chrono;

	_move_cooltime = steady_clock::now();
	_aatk_cooltime = steady_clock::now();
	_satk_cooltime = steady_clock::now();
	_datk_cooltime = steady_clock::now();
	Init();
}

void Game::Init()
{
	std::wcout.imbue(std::locale("korean"));			// 표준 출력 스트림에 적용

	if (not FontManager::Instance().LoadFont("neodot", "Resource/Font/neodgm.ttf")) {
		std::wcout << L"폰트 로드 실패\n";

		exit(-1);
	}

	TextureManager::Instance().LoadAllTextures("Resource/Texture/TextureSet.json");

	scene = std::make_shared<TitleScene>();

	// TODO : Loading이 끝나고 나서 last_time을 초기화 해야 함
	last_time = Epoch::now();
}

// 게임 실행 루프 (Input 받고, timer 업데이트, Scene Update, Draw)
void Game::Run()
{
	ConnectServer();

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
			Exit();
		}

		if (scene)
			scene->HandleInput(input_event);
	}
}

void Game::Exit()
{
	window.close();
}

void Game::SendID(const std::string& id)
{
	CS_LOGIN_PACKET login_packet{ id.c_str() };
	communication.Send(reinterpret_cast<BYTE*>(&login_packet), sizeof(login_packet));
}

void Game::SendRegister(uint8 class_type)
{
	CS_REGISTER_PACKET register_packet{ name.c_str(), class_type };
	communication.Send(reinterpret_cast<BYTE*>(&register_packet), sizeof(register_packet));
}

void Game::SendAttack(uint8 atk_key, uint8 atk_dir)
{
	using namespace std::chrono;

	auto now_point = steady_clock::now();
	switch (atk_key)
	{
	case KeyType::KEY_A:
		if ((now_point - _aatk_cooltime) < (AATK_COOLTIME)) {
			return;		// A공격 실패
		}
		_aatk_cooltime = now_point;
		break;
	case KeyType::KEY_S:
	{
		if ((now_point - _satk_cooltime) < (SATK_COOLTIME)) {
			return;		// S공격 실패
		}
		_satk_cooltime = now_point;
	}
	break;

	case KeyType::KEY_D:
	{
		if ((now_point - _datk_cooltime) < (DATK_COOLTIME)) {
			return;		// D공격 실패
		}
		_datk_cooltime = now_point;
	}
	break;
	}

	CS_ATTACK_PACKET attack_packet{ atk_key, atk_dir };
	communication.Send(reinterpret_cast<BYTE*>(&attack_packet), sizeof(attack_packet));
}

void Game::SendArrowKey(uint8 dir)
{
	using namespace std::chrono;

	auto now_time = steady_clock::now();
	if ((now_time - _move_cooltime) < (MOVE_COOLTIME)) {
		return;		// 이동 실패
	}
	_move_cooltime = now_time;

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
		scene = std::make_shared<TitleScene>();
		break;
	case SceneType::GameScene:
		scene = std::make_shared<GameScene>();
		break;
	case SceneType::LoginScene:
		scene = std::make_shared<LoginScene>();
		break;
	case SceneType::CreateScene:
		scene = std::make_shared<CreateScene>();
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

void Game::SetClassType(uint8 c_type)
{
	class_type = c_type;
}

uint8 Game::GetClassType() const
{
	return class_type;
}

void Game::SetName(const std::string& name_)
{
	name = name_;
}

const std::string& Game::GetName() const
{
	return name;
}
