#pragma once
#include "BaseScene.h"

// window에 관한 상수
constexpr int WINDOW_WIDTH{ 800 };
constexpr int WINDOW_HEIGHT{ 600 };


class Game
{
private:
	sf::RenderWindow window;

	std::shared_ptr<BaseScene> scene;

public:
	Game();

	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	void SceneLoad(SceneType scene_type);

};

