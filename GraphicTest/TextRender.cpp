#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // SFML â ����
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Example");

    // ��Ʈ ��ü ����
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) { // ��Ʈ ���� �ε�
        std::cerr << "��Ʈ �ε� ����!" << std::endl;
        return -1;
    }

    // �ؽ�Ʈ ��ü ���� �� ����
    sf::Text text;
    text.setFont(font);                // ��Ʈ ����
    text.setString("Hello, SFML!");    // ����� ���ڿ� ����
    text.setCharacterSize(50);         // ���� ũ��
    text.setFillColor(sf::Color::White); // ���� ����
    text.setStyle(sf::Text::Bold);     // ���� ����
    text.setPosition(200.f, 250.f);    // ��ġ ����

    // ���� ����
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ȭ�� ����� �ؽ�Ʈ �׸���
        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}
