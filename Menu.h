#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::string> m_items;
    int m_selected;
    sf::Font m_font;
    std::vector<sf::Text> m_texts;
    unsigned m_width, m_height;
public:
    Menu(unsigned width, unsigned height);
    void draw(sf::RenderTarget& target);
    void moveUp();
    void moveDown();
    int getSelected() const { return m_selected; }
    void processEvent(const sf::Event& ev); 
};
