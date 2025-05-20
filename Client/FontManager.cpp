#include "pch.h"
#include "FontManager.h"
#include <json/json.h>


// FontManager 싱글톤
FontManager& FontManager::Instance()
{
	static FontManager instance;
	return instance;
}

// key에 해당하는 폰트를 file_path의 파일로 불러오기
bool FontManager::LoadFont(const std::string& key, const std::string& file_path)
{
	// 이미 해당 텍스쳐가 있어서 false
	if (fonts.find(key) != fonts.end())
		return false;

	sf::Font font;
	// 텍스쳐 파일 로드에 실패할 경우
	if (not font.loadFromFile(file_path))
		throw std::runtime_error("Failed to load font: " + file_path);

	fonts[key] = std::move(font);
	return true;
}

// JSON 파일에 저장된 key와 경로를 이용해 모든 폰트 Load
void FontManager::LoadAllFonts(const std::string& json_file_path)
{
	std::ifstream file(json_file_path);

	if (not file.is_open())
		throw std::runtime_error("Failed to open JSON file: " + json_file_path);

	Json::Value root;
	file >> root;

	for (const auto& item : root["fonts"]) {
		std::string key = item["key"].asString();
		std::string path = item["path"].asString();

		LoadFont(key, path);
	}
}

// key에 해당하는 폰트 얻기
const sf::Font& FontManager::GetFont(const std::string& key) const
{
	auto it = fonts.find(key);
	if (it == fonts.end())
		throw std::runtime_error("Font not found: " + key);

	return it->second;
}
