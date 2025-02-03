#pragma once
#include "World.h"

constexpr double default_object_size{ 60.0 };

class World;


class Object
{
private:
	bool showing{ true };
	bool active{ true };

	double size;
	sf::Sprite sprite;
	sf::Text name_text;

	sf::Vector2i position;
	std::string name;

	std::weak_ptr<World> world;

public:
	Object();
	Object(std::shared_ptr<World> world);
	Object(sf::Texture& texture, int texture_x, int texture_y, int width, int height, std::shared_ptr<World> world);

private:
	bool Validate(int x, int y) const;

protected:
	void SetSize(double size);

	void SetSprite(const std::string& key);
	void SetSprite(const sf::Texture& texture);

public:
	void Show();
	void Hide();

	void Active();
	void Disactive();

	void Move(int x, int y);
	void Shift(int dx, int dy);

	void Draw(sf::RenderWindow& window);
	void SetName(const std::string& input_name);

};

