#pragma once


class BaseScene
{
protected:	// ���� ��ҵ� (������)
private:

private:
	virtual void Update(const int64 delta_time) = 0;
	virtual void Draw() = 0;
	virtual void HandleInput(const sf::Event& event) = 0;

protected:	// ���ӿ� �ʿ��� �Լ���
private:
public:

};

