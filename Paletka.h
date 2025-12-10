#pragma once
#include <SFML/Graphics.hpp>

class Paletka : public sf::RectangleShape {
private:
    float m_predkosc;
public:
    Paletka(sf::Vector2f pos = { 300.f, 520.f });
    void update(float dt);
    void setSpeed(float s) { m_predkosc = s; }
};
