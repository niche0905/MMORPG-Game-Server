#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // SFML 창 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Text Example");

    // 폰트 객체 생성
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF")) { // 폰트 파일 로드
        std::cerr << "폰트 로드 실패!" << std::endl;
        return -1;
    }

    // 텍스트 객체 생성 및 설정
    sf::Text text;
    text.setFont(font);                // 폰트 설정
    text.setString("Hello, SFML!");    // 출력할 문자열 설정
    text.setCharacterSize(50);         // 글자 크기
    text.setFillColor(sf::Color::White); // 글자 색상
    text.setStyle(sf::Text::Bold);     // 굵게 설정
    text.setPosition(200.f, 250.f);    // 위치 설정

    // 게임 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 화면 지우고 텍스트 그리기
        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }

    return 0;
}
