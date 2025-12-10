#include "Paletka.h"
#include <SFML/Window/Keyboard.hpp>

Paletka::Paletka(sf::Vector2f pos) : m_predkosc(450.f) {
    setSize({ 120.f, 18.f });
    setPosition(pos);
    setFillColor(sf::Color(180, 180, 255));
    setOrigin(getSize() / 2.f);
}

void Paletka::update(float dt) {
    float dx = 0.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  dx -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) dx += 1.f;
    move(sf::Vector2f{ dx * m_predkosc * dt, 0.f });
    float half = getSize().x / 2.f;
    auto pos = getPosition();
    if (pos.x - half < 0.f) setPosition({ half, pos.y });
    if (pos.x + half > 800.f) setPosition({ 800.f - half, pos.y });
}
