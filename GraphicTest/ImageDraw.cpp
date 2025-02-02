#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main() {
    // 1. SFML â ����
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Image Example");

    // 2. �ؽ�ó �ε�
    sf::Texture texture;
    if (!texture.loadFromFile("test0.png")) {  // �̹��� ��θ� ���� ���� ��η� ����
        std::cerr << "�̹����� �ҷ��� �� �����ϴ�!" << std::endl;
        return -1;
    }

    // 3. ��������Ʈ ���� �� �ؽ�ó ����
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // 4. ���� ����
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 5. ȭ�鿡 ��������Ʈ �׸���
        window.clear(sf::Color::Black);  // ����� ���������� �����
        window.draw(sprite);  // ��������Ʈ�� ȭ�鿡 �׸�
        window.display();  // ȭ�� ������Ʈ
    }

    return 0;
}
