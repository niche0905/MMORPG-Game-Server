#include "pch.h"
#include "Object.h"
#include "TextureManager.h"
#include "FontManager.h"


// 오브젝트 기본 생성자
Object::Object()
	: showing{ true }
	, active{ true }
	, size{ OBJECT_SIZE }
	, position{ 0, 0 }
{
	name_text.setFont(FontManager::Instance().GetFont("neodot"));
	name_text.setCharacterSize(18);
	name_text.setFillColor(sf::Color::White);
	name_text.setStyle(sf::Text::Regular);
	name_text.setOutlineColor(sf::Color::Black);
	name_text.setOutlineThickness(0.5f);
}

// 오브젝트 world 생성자
Object::Object(std::shared_ptr<World> world)
	: Object{}
{
	this->world = world;
}

// 오브젝트 텍스쳐 설정 생성자
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

		return 0 <= x and x < area.x and 0 <= y  and y < area.y;
	}

	return false;
}

void Object::SetSize(double size)
{
	this->size = size;
}

// 오브젝트 보이게
void Object::Show()
{
	showing = true;
}

// 오브젝트 안보이게
void Object::Hide()
{
	showing = false;
}

// 오브젝트 활성화 (움직이기, 상호작용 등)
void Object::Active()
{
	active = true;
}

// 오브젝트 비활성화
void Object::Disactive()
{
	active = false;
}

void Object::Update(const int64 delta_time)
{

}

// 오브젝트 위치 이동
void Object::Move(int x, int y)
{
	if (Validate(x, y))
		position = sf::Vector2i(x, y);
}

void Object::ForceMove(int x, int y)
{
	position = sf::Vector2i(x, y);
}

void Object::ForceMove(sf::Vector2i pos)
{
	position = pos;
}

// 오브젝트 위치 옮김
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

// 오브젝트 그리기
void Object::Draw(sf::RenderWindow& window)
{
	if (not showing) return;

	sf::Vector2f now_position(position.x * TILE_SIZE, position.y * TILE_SIZE);
	float offset = GetOffset();
	sf::Vector2f sprite_pos = now_position + sf::Vector2f{ offset, offset };
	sprite.setPosition(sprite_pos);

	window.draw(sprite);

	sf::Vector2f name_pos = now_position + sf::Vector2f{ TILE_SIZE / 2, (-TILE_SIZE / 2) + 5.0f };
	name_text.setPosition(name_pos);
	window.draw(name_text);
}

// 오브젝트 이름 설정
void Object::SetName(const std::string& input_name)
{
	name = input_name;
	name_text.setString(name);
	sf::FloatRect bounds = name_text.getLocalBounds();
	name_text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

sf::Vector2i Object::GetPosition() const
{
	return position;
}

float Object::GetOffset() const
{
	float offset = TILE_SIZE / 2 - size / 2;
	return offset;
}
