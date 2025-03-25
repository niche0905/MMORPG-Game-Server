#pragma once


enum class SceneType : uint8_t
{
	TitleScene,
	GameScene,
};


class BaseScene
{
protected:	// 게임 요소들 (변수들)


private:

public:
	virtual void Update(const int64 delta_time) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void HUD(sf::RenderWindow& window) = 0;
	virtual void HandleInput(const sf::Event& event) = 0;
	virtual void ProcessPacket(std::string packet) = 0;
	virtual sf::Vector2f GetCameraCenter() const = 0;

protected:	// 게임에 필요한 함수들
private:
public:

};

