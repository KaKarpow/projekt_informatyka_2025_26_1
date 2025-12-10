#pragma once
#include <SFML/Graphics.hpp>

class Pilka : public sf::CircleShape {
private:
    sf::Vector2f m_velocity;
    float m_speed; 

public:
    Pilka();

    void update(float dt);

    // Metody do fizyki
    void odbijX();
    void odbijY();
    void stop(); // Zatrzymanie (np. przy przegranej)
    void restart(); // Reset do stanu pocz¹tkowego

    // Gettery i Settery
    void setVelocity(sf::Vector2f v) { m_velocity = v; }
    sf::Vector2f velocity() const { return m_velocity; }
};