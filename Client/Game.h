#pragma once


class Game
{
private:
	sf::RenderWindow window;

	// TODO : Scene�� ������ �ְ� �ش� Scene�� �׷��ְ� �����־�� ��

private:
	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	// TODO : SceneLoad �Լ��� ���� ���� �ε� �ؾ� ��

};

