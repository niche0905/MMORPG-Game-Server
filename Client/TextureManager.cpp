#include "pch.h"
#include "TextureManager.h"
#include <json/json.h>


// TextureManager 싱글톤
TextureManager& TextureManager::Instance()
{
	static TextureManager instance;
	return instance;
}

// key에 해당하는 텍스쳐를 file_path의 파일로 불러오기
bool TextureManager::LoadTexture(const std::string& key, const std::string& file_path)
{
	// 이미 해당 텍스쳐가 있어서 false
	if (textures.find(key) != textures.end())
		return false;

	sf::Texture texture;
	// 텍스쳐 파일 로드에 실패할 경우
	if (not texture.loadFromFile(file_path))
		throw std::runtime_error("Failed to load texture: " + file_path);

	textures[key] = std::move(texture);
	return true;
}

// JSON 파일에 저장된 key와 경로를 이용해 모든 텍스쳐 Load
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

// key에 해당하는 텍스쳐 얻기
const sf::Texture& TextureManager::GetTexture(const std::string& key) const
{
	auto it = textures.find(key);
	if (it == textures.end())
		throw std::runtime_error("Texture not found: " + key);

	return it->second;
}
