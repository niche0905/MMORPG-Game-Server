#pragma once


class BaseScene
{
protected:	// 게임 요소들 (변수들)
private:

private:
	virtual void Update(const int64 delta_time) = 0;
	virtual void Draw() = 0;
	virtual void HandleInput(const sf::Event& event) = 0;

protected:	// 게임에 필요한 함수들
private:
public:

};

