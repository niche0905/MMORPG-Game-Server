#pragma once
#include "World.h"


class World;


class Object
{
protected:
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

public:
	void SetSize(double size);

	void SetSprite(const std::string& key);
	void SetSprite(const sf::Texture& texture);

	void SetScale();

public:
	void Show();
	void Hide();

	void Active();
	void Disactive();

	void Update(const int64 delta_time);

	void Move(int x, int y);
	void ForceMove(int x, int y);
	void ForceMove(sf::Vector2i pos);
	void Shift(int dx, int dy);

	void Draw(sf::RenderWindow& window);
	void SetName(const std::string& input_name);
	const std::string& GetName() const;
	std::string& GetName();

	sf::Vector2i GetPosition() const;
	float GetOffset() const;

};

