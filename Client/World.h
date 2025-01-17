#pragma once


class World
{
private:
	sf::Vector2i area;
	// TODO : Tile 만들고 Tile을 가지게 2000*2000 (처음엔 작게 100*100)

	
public:
	World();

	void Draw(sf::RenderWindow& window);

	void LoadWorld();
	void StoreWorld();

};

