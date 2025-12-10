#include "Pilka.h"
#include <cmath> 

Pilka::Pilka()
    : m_speed(350.f) // Startowa prêdkoœæ
{
    setRadius(8.f);
    setFillColor(sf::Color::White);

    
   
    setOrigin(sf::Vector2f(getRadius(), getRadius()));

    restart();
}

void Pilka::update(float dt) {
    
    move(m_velocity * dt);
}

void Pilka::odbijX() {
    m_velocity.x = -m_velocity.x;
}

void Pilka::odbijY() {
    m_velocity.y = -m_velocity.y;
}

void Pilka::stop() {
    m_velocity = sf::Vector2f(0.f, 0.f);
}

void Pilka::restart() {
    // Ustawiamy pi³kê na œrodku (400, 300)
    setPosition(sf::Vector2f(400.f, 300.f));

    // Startowa prêdkoœæ (lekko w bok, ¿eby nie spada³a pionowo)
    m_velocity = sf::Vector2f(200.f, -m_speed);
}