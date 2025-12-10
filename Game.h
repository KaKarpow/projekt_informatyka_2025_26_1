#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <algorithm> // do sortowania wyników
#include "Paletka.h"
#include "Pilka.h"
#include "Stone.h"
#include "PowerUp.h"
#include "Menu.h"


enum class GameState {
    Menu,
    Playing,
    GameOver,
    EnterName,   // Nowy stan: wpisywanie 3 liter
    Leaderboard  // Nowy stan: wyœwietlanie wyników
};

struct HighScore {
    std::string name;
    int score;
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void resetGame();      // Resetuje wszystko (nowa gra od 0)
    void nextLevel();      // Przechodzi do nast. poziomu (zachowuje wynik)

    void saveGame();
    void loadGame();

    // Obs³uga wyników
    void saveHighScore();       // Zapisuje wynik do pliku
    void loadHighScores();      // Wczytuje wyniki
    std::vector<HighScore> m_highScores;

    sf::RenderWindow m_window;
    sf::Font m_font;

    Paletka m_paletka;
    std::vector<Pilka> m_balls;
    std::vector<Stone> m_stones;
    std::vector<PowerUp> m_powerups;

    Menu m_menu;
    GameState m_state;

    int m_score = 0;
    int m_lives = 3;
    int m_level = 1; // Licznik poziomów

    // Do wpisywania nicku
    std::string m_playerName = "";

    sf::Text m_saveInfoText;
    float m_saveInfoTimer = 0.f;
};