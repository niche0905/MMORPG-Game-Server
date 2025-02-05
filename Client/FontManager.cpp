#include "pch.h"
#include "FontManager.h"
#include <json/json.h>


// FontManager �̱���
FontManager& FontManager::Instance()
{
	static FontManager instance;
	return instance;
}

// key�� �ش��ϴ� ��Ʈ�� file_path�� ���Ϸ� �ҷ�����
bool FontManager::LoadFont(const std::string& key, const std::string& file_path)
{
	// �̹� �ش� �ؽ��İ� �־ false
	if (fonts.find(key) != fonts.end())
		return false;

	sf::Font font;
	// �ؽ��� ���� �ε忡 ������ ���
	if (not font.loadFromFile(file_path))
		throw std::runtime_error("Failed to load font: " + file_path);

	fonts[key] = std::move(font);
	return true;
}

// JSON ���Ͽ� ����� key�� ��θ� �̿��� ��� ��Ʈ Load
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

// key�� �ش��ϴ� ��Ʈ ���
const sf::Font& FontManager::GetFont(const std::string& key) const
{
	auto it = fonts.find(key);
	if (it == fonts.end())
		throw std::runtime_error("Font not found: " + key);

	return it->second;
}
