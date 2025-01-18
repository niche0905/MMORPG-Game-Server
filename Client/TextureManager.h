#pragma once


class TextureManager
{
private:
	std::unordered_map<std::string, sf::Texture> textures;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

public:
	static TextureManager& Instance();

	bool LoadTexture(const std::string& key, const std::string& fild_path);
	const sf::Texture& GetTexture(const std::string& key) const;

};

