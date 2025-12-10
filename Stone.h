#pragma once
#include <SFML/Graphics.hpp>

class Stone : public sf::RectangleShape {
private:
    int m_type;      // Pe³ni funkcjê HP (3 = niebieski, 2 = ¿ó³ty, 1 = czerwony)
    bool m_destroyed;

public:

    Stone(sf::Vector2f pos, sf::Vector2f size, int type);

  
    void trafienie();           // Odejmuje ¿ycie
    bool czyZniszczony() const; // Sprawdza czy HP <= 0
	int getType() const;        // Getter typu (HP)


    void aktualizujKolor();
};