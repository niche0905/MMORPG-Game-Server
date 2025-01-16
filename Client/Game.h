#pragma once
#include "BaseScene.h"


class Game
{
private:
	sf::RenderWindow window;

	std::shared_ptr<BaseScene> scene;

private:
	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	void SceneLoad(SceneType scene_type);

};

