#include "pch.h"
#include "Object.h"
#include "TextureManager.h"


// ������Ʈ �⺻ ������
Object::Object()
	: showing{ true }
	, active{ true }
	, size{ default_object_size }
	, position{ 0, 0 }
{

}

// ������Ʈ world ������
Object::Object(std::shared_ptr<World> world)
	: Object{}
{
	this->world = world;
}

// ������Ʈ �ؽ��� ���� ������
Object::Object(sf::Texture& texture, int texture_x, int texture_y, int width, int height, std::shared_ptr<World> world)
	: Object{ world }
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(texture_x, texture_y, width, height));
}

bool Object::Validate(int x, int y) const
{
	if (auto w = world.lock()) {
		sf::Vector2i area = w->GetArea();

		return 0 <= x and x < area.x and 0 <= y < area.y;
	}

	return false;
}

void Object::SetSize(double size)
{
	this->size = size;
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

// ������Ʈ Ȱ��ȭ (�����̱�, ��ȣ�ۿ� ��)
void Object::Active()
{
	active = true;
}

// ������Ʈ ��Ȱ��ȭ
void Object::Disactive()
{
	active = false;
}

// ������Ʈ ��ġ �̵�
void Object::Move(int x, int y)
{
	if (Validate(x, y))
		position = sf::Vector2i(x, y);
}

// ������Ʈ ��ġ �ű�
void Object::Shift(int dx, int dy)
{
	if (Validate(position.x + dx, position.y + dy))
		position += sf::Vector2i(dx, dy);
}

void Object::SetSprite(const std::string& key)
{
	sprite.setTexture(TextureManager::Instance().GetTexture(key));
	SetScale();
}

void Object::SetSprite(const sf::Texture& texture)
{
	sprite.setTexture(texture);
	SetScale();
}

void Object::SetScale()
{
	sf::Vector2u texture_size = sprite.getTexture()->getSize();

	sprite.setScale(size / texture_size.x, size / texture_size.y);
}

// ������Ʈ �׸���
void Object::Draw(sf::RenderWindow& window)
{
	if (not showing) return;

	sf::Vector2f draw_position(position.x * tile_size, position.y * tile_size);
	float offset = tile_size / 2 + size / 2;
	draw_position += sf::Vector2f{ offset, offset };

	window.draw(sprite);

	// TODO : name�� �ִٸ� ���

}

// ������Ʈ �̸� ����
void Object::SetName(const std::string& input_name)
{
	// TODO : name ����
}
