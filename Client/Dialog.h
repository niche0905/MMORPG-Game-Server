#pragma once


class Dialog
{
private:
	bool					_is_active = false;

	sf::RectangleShape		_dialog_box;

	sf::Text				_text;
	sf::Text				_yes_text;
	sf::Text				_no_text;

	uint32					_button_index;
	uint32					_max_index;
	sf::RectangleShape		_button_box;

public:
	Dialog();

	void HandleInput(const sf::Event& event);
	void UpdateButtonSelect();
	void Draw(sf::RenderWindow& window);

	void SetActive(bool active);
	bool IsActive() const;

};

