#pragma once
#include "Communication.h"
#include "BaseScene.h"

// window�� ���� ���
constexpr int WINDOW_WIDTH{ 600 };
constexpr int WINDOW_HEIGHT{ 600 };

class BaseScene;
class Communication;


class Game
{
private:
	using Window = sf::RenderWindow;
	using Epoch = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Epoch>;
	using MicroSeconds = std::chrono::microseconds;

private:
	Window window;
	sf::View view;

	std::shared_ptr<BaseScene> scene;

	TimePoint last_time;

	Communication communication;

	uint64_t my_id;

public:
	Game();

	void Init();

	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	void SendArrowKey(char dir);
	void ProcessPacket(std::vector<char> packet);

	void ConnectServer();

	void SetCameraView();

	void SceneLoad(SceneType scene_type);

	void SetID(uint64_t id);
	uint64_t GetID() const;

};

