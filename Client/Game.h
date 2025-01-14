#pragma once


class Game
{
private:
	sf::RenderWindow window;

	// TODO : Scene을 가지고 있고 해당 Scene을 그려주고 돌려주어야 함

private:
	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	// TODO : SceneLoad 함수를 만들어서 씬을 로드 해야 함

};

