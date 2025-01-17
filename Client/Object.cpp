#include "pch.h"
#include "Object.h"


// ������Ʈ �⺻ ������
Object::Object()
	: showing{ true }
	, position{ 0,0 }
{

}

// ������Ʈ �ؽ��� ���� ������
Object::Object(sf::Texture& texture, int texture_x, int texture_y, int width, int height)
	: showing{ true }
	, position{ 0,0 }
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(texture_x, texture_y, width, height));
}

// ������Ʈ ���̰�
void Object::Show()
{
	showing = true;
}

// ������Ʈ �Ⱥ��̰�
void Object::Hide()
{
	showing = false;
}

// ������Ʈ ��ġ �̵�
void Object::Move(int x, int y)
{
	position = sf::Vector2i(x, y);
}

// ������Ʈ ��ġ �ű�
void Object::Shift(int dx, int dy)
{
	position += sf::Vector2i(dx, dy);

	// TODO : validate check


}

// ������Ʈ �׸���
void Object::Draw(sf::RenderWindow& window)
{
	if (not showing) return;

	// TODO : ü���ǿ��� ��ġ ��� �� ��ġ

	window.draw(sprite);

	// TODO : name�� �ִٸ� ���

}

// ������Ʈ �̸� ����
void Object::SetName(const std::string& input_name)
{
	// TODO : name ����
}
