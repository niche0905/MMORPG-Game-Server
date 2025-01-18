#include "pch.h"
#include "TextureManager.h"
#include <json/json.h>


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

// JSON ���Ͽ� ����� key�� ��θ� �̿��� ��� �ؽ��� Load
void TextureManager::LoadAllTextures(const std::string& json_file_path)
{
	std::ifstream file(json_file_path);

	if(not file.is_open())
		throw std::runtime_error("Failed to open JSON file: " + json_file_path);

	Json::Value root;
	file >> root;

	for (const auto& item : root["textures"]) {
		std::string key = item["key"].asString();
		std::string path = item["path"].asString();

		LoadTexture(key, path);
	}
}

// key�� �ش��ϴ� �ؽ��� ���
const sf::Texture& TextureManager::GetTexture(const std::string& key) const
{
	auto it = textures.find(key);
	if (it == textures.end())
		throw std::runtime_error("Texture not found: " + key);

	return it->second;
}
