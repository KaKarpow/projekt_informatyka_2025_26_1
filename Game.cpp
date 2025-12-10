#include "Game.h"
#include <random>
#include <sstream>
#include <iostream>

// Sortowanie wyników malej¹co
bool compareScores(const HighScore& a, const HighScore& b) {
    return a.score > b.score;
}

Game::Game()
    : m_window(sf::VideoMode({ 800, 600 }), "Arkanoid FINAL EDITION", sf::Style::Close | sf::Style::Titlebar)
    , m_paletka({ 400.f, 560.f })
    , m_menu(800, 600)
    , m_state(GameState::Menu)
    , m_saveInfoText(m_font, "GRA ZAPISANA!", 24)
{
    m_window.setFramerateLimit(60);

    if (!m_font.openFromFile("arial.ttf")) {
        std::cerr << "B³¹d: Nie znaleziono arial.ttf!\n";
    }

    m_saveInfoText.setFillColor(sf::Color::Green);
    m_saveInfoText.setPosition({ 20.f, 550.f });

    resetGame();
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        processEvents();

        sf::Time dtTime = clock.restart();
        float dt = dtTime.asSeconds();

        if (m_state == GameState::Playing) update(dt);
        if (m_saveInfoTimer > 0.f) m_saveInfoTimer -= dt;

        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) m_window.close();

        // --- OBS£UGA WPISYWANIA NICKU ---
        if (m_state == GameState::EnterName) {
            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                // Obs³uga tylko znaków ASCII (A-Z, 0-9)
                if (textEvent->unicode < 128) {
                    char c = static_cast<char>(textEvent->unicode);

                    // Backspace 
                    if (c == 8 && !m_playerName.empty()) {
                        m_playerName.pop_back();
                    }
                    // Enter 
                    else if (c == 13) {
                        if (!m_playerName.empty()) {
                            saveHighScore(); // Zapisz wynik
                            loadHighScores(); // Odœwie¿ listê
                            m_state = GameState::Leaderboard; // Poka¿ tabelê
                        }
                    }
                    // Litery i cyfry (limit 3 znaki)
                    else if (isalnum(c) && m_playerName.size() < 3) {
                        m_playerName += (char)toupper(c);
                    }
                }
            }
        }

        // --- OBS£UGA KLAWISZY FUNKCYJNYCH ---
        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {

            // ESCAPE - powroty
            if (key->scancode == sf::Keyboard::Scancode::Escape) {
                if (m_state == GameState::Playing) m_state = GameState::Menu;
                else if (m_state == GameState::Leaderboard) m_state = GameState::Menu;
                else m_window.close();
            }

            // F5 - Zapis (tylko w grze)
            if (key->scancode == sf::Keyboard::Scancode::F5 && m_state == GameState::Playing) {
                saveGame();
            }

            // MENU G£ÓWNE
            if (m_state == GameState::Menu) {
                m_menu.processEvent(*event);
                if (key->scancode == sf::Keyboard::Scancode::Enter || key->scancode == sf::Keyboard::Scancode::Space) {
                    int selected = m_menu.getSelected();
                    if (selected == 0) { // Nowa Gra
                        resetGame();
                        m_state = GameState::Playing;
                    }
                    else if (selected == 1) { // Wczytaj
                        loadGame();
                    }
                    else if (selected == 2) { // Wyniki (High Score)
                        loadHighScores();
                        m_state = GameState::Leaderboard;
                    }
                    else if (selected == 3) { // Wyjœcie
                        m_window.close();
                    }
                }
            }
            // TABLICA WYNIKÓW (Wyjœcie enterem lub spacj¹)
            else if (m_state == GameState::Leaderboard) {
                if (key->scancode == sf::Keyboard::Scancode::Enter || key->scancode == sf::Keyboard::Scancode::Space) {
                    m_state = GameState::Menu;
                }
            }
        }
    }
}

void Game::update(float dt) {
    m_paletka.update(dt);

    // --- LOGIKA PI£EK ---
    for (auto itBall = m_balls.begin(); itBall != m_balls.end();) {
        itBall->update(dt);

        auto pos = itBall->getPosition();
        float r = itBall->getRadius();

        // Œciany
        if (pos.x - r < 0.f) { itBall->setPosition({ r, pos.y }); itBall->odbijX(); }
        else if (pos.x + r > 800.f) { itBall->setPosition({ 800.f - r, pos.y }); itBall->odbijX(); }
        if (pos.y - r < 0.f) { itBall->setPosition({ pos.x, r }); itBall->odbijY(); }

        // Œmieræ pi³ki
        if (pos.y > 650.f) {
            itBall = m_balls.erase(itBall);
            continue;
        }

        // Paletka
        if (itBall->getGlobalBounds().findIntersection(m_paletka.getGlobalBounds())) {
            if (itBall->velocity().y > 0.f) {
                itBall->odbijY();
                float palHalfH = m_paletka.getSize().y / 2.f;
                itBall->setPosition({ itBall->getPosition().x, m_paletka.getPosition().y - palHalfH - r - 0.1f });

                // Podkrêcanie
                float diff = itBall->getPosition().x - m_paletka.getPosition().x;
                auto v = itBall->velocity();
                v.x = diff * 4.5f;
                itBall->setVelocity(v);
            }
        }

        // Klocki
        bool hitBlock = false;
        static std::mt19937 rng((unsigned)std::random_device{}());
        std::uniform_int_distribution<int> chance(0, 2);
        std::uniform_int_distribution<int> ptype(0, 2);

        for (auto itStone = m_stones.begin(); itStone != m_stones.end();) {
            if (!hitBlock && itBall->getGlobalBounds().findIntersection(itStone->getGlobalBounds())) {
                itStone->trafienie();
                itBall->odbijY();
                hitBlock = true;

                if (itStone->czyZniszczony()) {
                    if (chance(rng) == 0) {
                        PowerType t = static_cast<PowerType>(ptype(rng));
                        m_powerups.emplace_back(t, itStone->getPosition());
                    }
                    itStone = m_stones.erase(itStone);
                    m_score += 10 * m_level; // Wiêcej punktów na wy¿szych levelach!
                }
                else {
                    ++itStone;
                }
            }
            else {
                ++itStone;
            }
        }
        ++itBall;
    }

    // --- UTRATA ¯YCIA / GAME OVER ---
    if (m_balls.empty()) {
        m_lives--;
        if (m_lives <= 0) {
            m_playerName = ""; 
            m_state = GameState::EnterName;
        }
        else {
            Pilka p; p.restart(); m_balls.push_back(p);
            m_powerups.clear();
        }
    }

    // --- NASTÊPNY POZIOM (NIESKOÑCZONOŒÆ) ---
    if (m_stones.empty()) {
        nextLevel();
    }

    // --- POWERUPY ---
    for (auto it = m_powerups.begin(); it != m_powerups.end();) {
        it->update(dt);
        if (it->getPosition().y > 650.f) { it = m_powerups.erase(it); continue; }

        if (it->getGlobalBounds().findIntersection(m_paletka.getGlobalBounds())) {
            switch (it->type) {
            case PowerType::ExpandPaddle: {
                auto s = m_paletka.getSize(); s.x = std::min(300.f, s.x + 40.f);
                m_paletka.setSize(s); m_paletka.setOrigin({ s.x / 2.f, s.y / 2.f });
                break;
            }
            case PowerType::SlowBall: {
                for (auto& b : m_balls) { auto v = b.velocity(); v *= 0.8f; b.setVelocity(v); }
                break;
            }
            case PowerType::MultiBall: {
                Pilka nb; nb.setPosition({ m_paletka.getPosition().x, m_paletka.getPosition().y - 20.f });
                nb.setVelocity({ (rand() % 2 ? 200.f : -200.f), -300.f });
                m_balls.push_back(nb);
                break;
            }
            }
            it = m_powerups.erase(it);
        }
        else { ++it; }
    }
}

void Game::render() {
    m_window.clear();

    if (m_state == GameState::Menu) {
        m_menu.draw(m_window);
        sf::Text t(m_font, "ARKANOID - PG Project", 50);
        t.setFillColor(sf::Color::Cyan);
        sf::FloatRect r = t.getLocalBounds();
        t.setOrigin({ r.size.x / 2.f, r.size.y / 2.f });
        t.setPosition({ 400.f, 100.f });
        m_window.draw(t);
    }
    else if (m_state == GameState::Playing) {
        m_window.draw(m_paletka);
        for (auto& b : m_balls) m_window.draw(b);
        for (auto& s : m_stones) m_window.draw(s);
        for (auto& p : m_powerups) m_window.draw(p);

        sf::Text t(m_font, "", 20);
        t.setFillColor(sf::Color::White);
        std::ostringstream ss;
        ss << "Score: " << m_score << "   Lives: " << m_lives
            << "   Level: " << m_level; // Wyœwietlamy poziom
        t.setString(ss.str());
        t.setPosition({ 10.f, 10.f });
        m_window.draw(t);

        if (m_saveInfoTimer > 0.f) m_window.draw(m_saveInfoText);
    }
    else if (m_state == GameState::EnterName) {
        // EKRAN WPISYWANIA NICKU
        sf::Text title(m_font, "GAME OVER", 60);
        title.setFillColor(sf::Color::Red);
        sf::FloatRect tr = title.getLocalBounds();
        title.setOrigin({ tr.size.x / 2.f, tr.size.y / 2.f });
        title.setPosition({ 400.f, 150.f });

        sf::Text scoreInfo(m_font, "Twoj wynik: " + std::to_string(m_score), 30);
        scoreInfo.setFillColor(sf::Color::White);
        sf::FloatRect sr = scoreInfo.getLocalBounds();
        scoreInfo.setOrigin({ sr.size.x / 2.f, sr.size.y / 2.f });
        scoreInfo.setPosition({ 400.f, 250.f });

        sf::Text prompt(m_font, "Wpisz 3 litery i wcisnij ENTER:", 20);
        prompt.setFillColor(sf::Color::Yellow);
        sf::FloatRect pr = prompt.getLocalBounds();
        prompt.setOrigin({ pr.size.x / 2.f, pr.size.y / 2.f });
        prompt.setPosition({ 400.f, 350.f });

        sf::Text nameText(m_font, m_playerName, 50);
        nameText.setFillColor(sf::Color::Cyan);
        sf::FloatRect nr = nameText.getLocalBounds();
        nameText.setOrigin({ nr.size.x / 2.f, nr.size.y / 2.f });
        nameText.setPosition({ 400.f, 420.f });

        m_window.draw(title);
        m_window.draw(scoreInfo);
        m_window.draw(prompt);
        m_window.draw(nameText);
    }
    else if (m_state == GameState::Leaderboard) {
        // EKRAN WYNIKÓW
        sf::Text title(m_font, "NAJLEPSZE WYNIKI", 40);
        title.setFillColor(sf::Color::Yellow);
        sf::FloatRect tr = title.getLocalBounds();
        title.setOrigin({ tr.size.x / 2.f, tr.size.y / 2.f });
        title.setPosition({ 400.f, 50.f });
        m_window.draw(title);

        float y = 130.f;
        int place = 1;
        for (const auto& hs : m_highScores) {
            std::ostringstream ss;
            ss << place << ". " << hs.name << "   ...........   " << hs.score;
            sf::Text line(m_font, ss.str(), 30);
            line.setFillColor(sf::Color::White);
            if (place == 1) line.setFillColor(sf::Color::Cyan); // Mistrz na cyjanowo

            sf::FloatRect lr = line.getLocalBounds();
            line.setOrigin({ lr.size.x / 2.f, lr.size.y / 2.f });
            line.setPosition({ 400.f, y });
            m_window.draw(line);

            y += 50.f;
            place++;
            if (place > 5) break; // Tylko top 5
        }

        sf::Text info(m_font, "Wcisnij ENTER aby wrocic", 20);
        info.setPosition({ 280.f, 550.f });
        m_window.draw(info);
    }

    m_window.display();
}

void Game::resetGame() {
    m_score = 0;
    m_lives = 3;
    m_level = 1;
    m_balls.clear();

    Pilka p; p.restart(); m_balls.push_back(p);

    // Generowanie poziomu 1
    nextLevel();
    m_level = 1; // nextLevel podbija licznik, wiêc resetujemy go na 1
}

void Game::nextLevel() {
    m_stones.clear();
    m_powerups.clear();
    m_balls.clear();

    Pilka p; p.restart(); m_balls.push_back(p);
    m_paletka.setPosition({ 400.f, 560.f });

    const int rows = 5;
    const int cols = 10;
    const sf::Vector2f size{ 72.f, 24.f };
    const float pad = 4.f;

    // GENEROWANIE KLOCKÓW ZALE¯NE OD POZIOMU
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            // Im wy¿szy poziom, tym wiêksza szansa na twarde klocki
            int type = 1;
            int randVal = rand() % 100;

            if (m_level == 1) {
                type = (r % 2) + 1; // Proste klocki (1 i 2 ¿ycie)
            }
            else if (m_level == 2) {
                type = (r % 3) + 1; // Do 3 ¿ycia
            }
            else {
                // Od 3 poziomu losowo, ale wiêcej twardych
                if (randVal < 30) type = 3;
                else if (randVal < 60) type = 2;
                else type = 1;

                // Na bardzo wysokich poziomach (5+) same twarde
                if (m_level >= 5 && randVal < 50) type = 3;
            }

            m_stones.emplace_back(
                sf::Vector2f(40.f + c * (size.x + pad), 60.f + r * (size.y + pad)),
                size,
                type
            );
        }
    }

    m_level++; // Zwiêkszamy licznik (¿eby przy kolejnym resecie by³o trudniej)
}

void Game::saveGame() {
    // Stara funkcja zapisu (bez zmian w logice, ale zapisuje poziom)
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << m_score << " " << m_lives << " " << m_level << "\n"; // Dodano poziom
        file << m_balls.size() << "\n";
        for (const auto& b : m_balls) {
            file << b.getPosition().x << " " << b.getPosition().y << " "
                << b.velocity().x << " " << b.velocity().y << "\n";
        }
        file << m_paletka.getPosition().x << " " << m_paletka.getPosition().y << " " << m_paletka.getSize().x << "\n";
        file << m_stones.size() << "\n";
        for (const auto& s : m_stones) {
            file << s.getPosition().x << " " << s.getPosition().y << " " << s.getType() << "\n";
        }
        file.close();
        m_saveInfoTimer = 2.0f;
    }
}

void Game::loadGame() {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        m_stones.clear(); m_powerups.clear(); m_balls.clear();
        file >> m_score >> m_lives >> m_level; // Wczytujemy poziom

        size_t bc; file >> bc;
        for (size_t i = 0; i < bc; ++i) {
            float bx, by, bvx, bvy; file >> bx >> by >> bvx >> bvy;
            Pilka p; p.setPosition({ bx,by }); p.setVelocity({ bvx,bvy });
            m_balls.push_back(p);
        }
        float px, py, pw; file >> px >> py >> pw;
        m_paletka.setSize({ pw,18.f }); m_paletka.setOrigin({ pw / 2.f, 9.f }); m_paletka.setPosition({ px,py });

        size_t sc; file >> sc;
        sf::Vector2f sz{ 72.f,24.f };
        for (size_t i = 0; i < sc; ++i) {
            float sx, sy; int st; file >> sx >> sy >> st;
            m_stones.emplace_back(sf::Vector2f(sx, sy), sz, st);
        }
        file.close();
        m_state = GameState::Playing;
    }
}

// --- ZAPIS I ODCZYT WYNIKÓW (HIGHSCORES) ---

void Game::saveHighScore() {
    // 1. Wczytaj stare wyniki
    loadHighScores();

    // 2. Dodaj nowy wynik
    m_highScores.push_back({ m_playerName, m_score });

    // 3. Posortuj
    std::sort(m_highScores.begin(), m_highScores.end(), compareScores);

    // 4. Zapisz top 10 do pliku
    std::ofstream file("highscores.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < m_highScores.size() && i < 10; ++i) {
            file << m_highScores[i].name << " " << m_highScores[i].score << "\n";
        }
    }
}

void Game::loadHighScores() {
    m_highScores.clear();
    std::ifstream file("highscores.txt");
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            m_highScores.push_back({ name, score });
        }
    }
    // Sortuj dla pewnoœci przy wyœwietlaniu
    std::sort(m_highScores.begin(), m_highScores.end(), compareScores);
}