#pragma once
#include "BaseScene.h"

// window에 관한 상수
constexpr int WINDOW_WIDTH{ 600 };
constexpr int WINDOW_HEIGHT{ 600 };

class BaseScene;


class Game
{
private:
	using Window = sf::RenderWindow;
	using Epoch = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Epoch>;
	using MicroSeconds = std::chrono::microseconds;

private:
	Window window;

	std::shared_ptr<BaseScene> scene;

	TimePoint last_time;

public:
	Game();

	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	void SceneLoad(SceneType scene_type);

};

