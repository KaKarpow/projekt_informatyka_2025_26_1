#include "PowerUp.h"

PowerUp::PowerUp(PowerType t, sf::Vector2f pos) : type(t), velocity({ 0.f, 150.f }) { // Szybsze spadanie
    setRadius(10.f);
    setOrigin({ getRadius(), getRadius() });
    setPosition(pos);

    switch (type) {
    case PowerType::ExpandPaddle: setFillColor(sf::Color::Green); break;
    case PowerType::SlowBall:     setFillColor(sf::Color::Yellow); break;
    case PowerType::MultiBall:    setFillColor(sf::Color::Cyan); break; // MultiBall na cyjan
    }
}

void PowerUp::update(float dt) {
    move(velocity * dt);
}