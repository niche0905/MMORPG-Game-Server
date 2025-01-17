#include "pch.h"
#include "Object.h"


// 오브젝트 기본 생성자
Object::Object()
	: showing{ true }
	, position{ 0,0 }
{

}

// 오브젝트 텍스쳐 설정 생성자
Object::Object(sf::Texture& texture, int texture_x, int texture_y, int width, int height)
	: showing{ true }
	, position{ 0,0 }
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(texture_x, texture_y, width, height));
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

// 오브젝트 위치 이동
void Object::Move(int x, int y)
{
	position = sf::Vector2i(x, y);
}

// 오브젝트 위치 옮김
void Object::Shift(int dx, int dy)
{
	position += sf::Vector2i(dx, dy);

	// TODO : validate check


}

// 오브젝트 그리기
void Object::Draw(sf::RenderWindow& window)
{
	if (not showing) return;

	// TODO : 체스판에서 위치 계산 후 배치

	window.draw(sprite);

	// TODO : name이 있다면 출력

}

// 오브젝트 이름 설정
void Object::SetName(const std::string& input_name)
{
	// TODO : name 설정
}
