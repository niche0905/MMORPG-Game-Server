#pragma once


class World
{
private:
	sf::Vector2i area;
	// TODO : Tile ����� Tile�� ������ 2000*2000 (ó���� �۰� 100*100)

	
public:
	World();

	void Draw(sf::RenderWindow& window);

	void LoadWorld();
	void StoreWorld();

};

