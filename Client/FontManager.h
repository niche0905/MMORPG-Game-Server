#pragma once


class FontManager
{
private:
	std::unordered_map<std::string, sf::Font> fonts;

	FontManager() = default;
	~FontManager() = default;
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

public:
	static FontManager& Instance();

	bool LoadFont(const std::string& key, const std::string& file_path);
	void LoadAllFonts(const std::string& json_file_path);
	const sf::Font& GetFont(const std::string& key) const;

};

