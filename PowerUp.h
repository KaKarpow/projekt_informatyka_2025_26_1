#pragma once
#include <SFML/Graphics.hpp>


enum class PowerType { ExpandPaddle, SlowBall, MultiBall };

class PowerUp : public sf::CircleShape {
public:
    PowerType type;
    sf::Vector2f velocity;

    PowerUp(PowerType t, sf::Vector2f pos);
    void update(float dt);
};