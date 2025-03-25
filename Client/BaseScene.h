#pragma once


enum class SceneType : uint8_t
{
	TitleScene,
	GameScene,
};


class BaseScene
{
protected:	// ���� ��ҵ� (������)


private:

public:
	virtual void Update(const int64 delta_time) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void HUD(sf::RenderWindow& window) = 0;
	virtual void HandleInput(const sf::Event& event) = 0;
	virtual void ProcessPacket(std::string packet) = 0;
	virtual sf::Vector2f GetCameraCenter() const = 0;

protected:	// ���ӿ� �ʿ��� �Լ���
private:
public:

};

