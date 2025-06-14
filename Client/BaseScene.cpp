#include "pch.h"
#include "BaseScene.h"

sf::Vector2f BaseScene::GetCameraCenter() const
{
	return { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f };
}
