#include "Stone.h"

Stone::Stone(sf::Vector2f pos, sf::Vector2f size, int type)
    : m_type(type), m_destroyed(false)
{
    setPosition(pos);
    setSize(size);
    setOutlineColor(sf::Color::Black);
    setOutlineThickness(1.f);

    // Ustawiamy kolor startowy w zale¿noœci od typu (¿ycia)
    aktualizujKolor();
}

void Stone::trafienie() {
    if (m_destroyed) return;

    m_type--; // Odejmujemy jedno ¿ycie

    if (m_type <= 0) {
        m_destroyed = true; // Zniszczony
    }
    else {
        aktualizujKolor(); // Jeszcze ¿yje, ale zmieniamy kolor
    }
}

bool Stone::czyZniszczony() const {
    return m_destroyed;
}

int Stone::getType() const {
    return m_type;
}

void Stone::aktualizujKolor() {
    // Logika kolorów:
    // 1 HP = Czerwony (krytyczny)
    // 2 HP = ¯ó³ty
    // 3 HP = Niebieski
    // 4+ HP = Zielony

    switch (m_type) {
    case 1: setFillColor(sf::Color::Red); break;
    case 2: setFillColor(sf::Color::Yellow); break;
    case 3: setFillColor(sf::Color::Blue); break;
    default: setFillColor(sf::Color::Green); break;
    }
}