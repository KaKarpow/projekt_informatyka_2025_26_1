#include "Menu.h"
#include <iostream>

Menu::Menu(unsigned width, unsigned height)
    : m_selected(0), m_width(width), m_height(height)
{
    m_items = { "Nowa gra","Wczytaj gre", "Wyniki", "Wyjscie" };
    if (!m_font.openFromFile("arial.ttf")) {
        std::cerr << "Nie mo¿na wczytaæ 'arial.ttf' — u¿ywam domyœlnej czcionki SFML\n";
     
    }

    float y = 150.f;
    for (const auto& it : m_items) {
      
        sf::Text t(m_font, it, 28u);
        t.setPosition(sf::Vector2f((float)width / 2.f - 80.f, y)); 
        t.setFillColor(sf::Color::White);
        m_texts.push_back(std::move(t));
        y += 50.f;
    }
    if (!m_texts.empty()) m_texts[m_selected].setFillColor(sf::Color::Yellow);
}

void Menu::draw(sf::RenderTarget& target) {
    for (auto& t : m_texts) target.draw(t);
}

void Menu::moveUp() {
    if (m_texts.empty()) return;
    m_texts[m_selected].setFillColor(sf::Color::White);
    m_selected = (m_selected - 1 + static_cast<int>(m_texts.size())) % static_cast<int>(m_texts.size());
    m_texts[m_selected].setFillColor(sf::Color::Yellow);
}
void Menu::moveDown() {
    if (m_texts.empty()) return;
    m_texts[m_selected].setFillColor(sf::Color::White);
    m_selected = (m_selected + 1) % static_cast<int>(m_texts.size());
    m_texts[m_selected].setFillColor(sf::Color::Yellow);
}

void Menu::processEvent(const sf::Event& ev) {
    
    if (ev.is<sf::Event::KeyPressed>()) {
        if (auto kp = ev.getIf<sf::Event::KeyPressed>()) {
            if (kp->code == sf::Keyboard::Key::Up) moveUp();
            else if (kp->code == sf::Keyboard::Key::Down) moveDown();
        }
    }
}

