#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

int main() {
    // 1. SFML 창 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Image Example");

    // 2. 텍스처 로드
    sf::Texture texture;
    if (!texture.loadFromFile("warrior_s.png")) {  // 이미지 경로를 실제 파일 경로로 변경
        std::cerr << "이미지를 불러올 수 없습니다!" << std::endl;
        return -1;
    }

    // 3. 스프라이트 생성 및 텍스처 적용
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);

    // 4. 게임 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 5. 화면에 스프라이트 그리기
        window.clear(sf::Color::Black);  // 배경을 검은색으로 지우기
        window.draw(sprite);  // 스프라이트를 화면에 그림
        window.display();  // 화면 업데이트
    }

    sf::Vector2u texture_size = sprite.getTexture()->getSize();
    std::cout << texture_size.x << ", " << texture_size.y << '\n';

    return 0;
}
