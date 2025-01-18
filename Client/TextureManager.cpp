#include "pch.h"
#include "TextureManager.h"


// TextureManager �̱���
TextureManager& TextureManager::Instance()
{
	static TextureManager instance;
	return instance;
}

// key�� �ش��ϴ� �ؽ��ĸ� file_path�� ���Ϸ� �ҷ�����
bool TextureManager::LoadTexture(const std::string& key, const std::string& file_path)
{
	// �̹� �ش� �ؽ��İ� �־ false
	if (textures.find(key) != textures.end())
		return false;

	sf::Texture texture;
	// �ؽ��� ���� �ε忡 ������ ���
	if (not texture.loadFromFile(file_path))
		throw std::runtime_error("Failed to load texture: " + file_path);

	textures[key] = std::move(texture);
	return true;
}

// key�� �ش��ϴ� �ؽ��� ���
const sf::Texture& TextureManager::GetTexture(const std::string& key) const
{
	auto it = textures.find(key);
	if (it == textures.end())
		throw std::runtime_error("Texture not found: " + key);

	return it->second;
}
