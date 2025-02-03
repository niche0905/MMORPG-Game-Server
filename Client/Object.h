#pragma once
#include "World.h"

class World;


class Object
{
private:
	bool showing{ true };
	bool active{ true };

	sf::Sprite sprite;
	sf::Text name_text;

	sf::Vector2i position;
	std::string name;

	std::shared_ptr<World> world;

public:
	Object();
	Object(sf::Texture& texture, int texture_x, int texture_y, int width, int height);

	void Show();
	void Hide();

	void Active();
	void Disactive();

	void Move(int x, int y);
	void Shift(int dx, int dy);

	void Draw(sf::RenderWindow& window);
	void SetName(const std::string& input_name);

};

