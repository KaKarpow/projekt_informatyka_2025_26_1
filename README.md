#  Arkanoid - PG Project (SFML 3.0.0)

![C++](https://img.shields.io/badge/C++-17-blue.svg) ![SFML](https://img.shields.io/badge/SFML-3.0.0-green.svg) ![Status](https://img.shields.io/badge/Status-Ukończony-success.svg) ![License](https://img.shields.io/badge/License-MIT-yellow.svg)

> **Zaawansowana implementacja klasycznej gry arcade napisana w nowoczesnym C++ z użyciem najnowszej biblioteki SFML 3.0.0.**

Projekt został zrealizowany jako część zaliczenia przedmiotu Informatyka 2. Wyróżnia się rozbudowaną mechaniką (system Multi-Ball, nieskończone poziomy), pełną obsługą zapisu stanu gry oraz tablicą najlepszych wyników. Kod źródłowy jest w pełni obiektowy i wykorzystuje kontenery STL.

## 📸 Zrzut ekranu
<img width="796" height="590" alt="image" src="https://github.com/user-attachments/assets/9156202f-b7b7-4021-963c-faa548002cde" />


## 🚀 Główne Funkcje

### 🎮 Mechanika Rozgrywki
* **Nieskończona Progresja:** Gra nie ma końca. Po wyczyszczeniu planszy automatycznie generowany jest kolejny poziom. Z każdym poziomem (Level 1, 2, 3...) wzrasta trudność – pojawia się więcej klocków o wyższej wytrzymałości.
* **System Multi-Ball:** Dynamiczne zarządzanie wieloma piłkami naraz. Gracz traci życie dopiero w momencie, gdy wszystkie aktywne piłki spadną poniżej dolnej krawędzi.
* **Wytrzymałość Klocków (HP):** Klocki posiadają punkty życia wizualizowane kolorami:
    * 🟦 **Niebieski:** 3 uderzenia (Najtwardszy)
    * 🟨 **Żółty:** 2 uderzenia
    * 🟥 **Czerwony:** 1 uderzenie (Podstawowy)
* **Zaawansowana Fizyka:** Precyzyjne wykrywanie kolizji oraz mechanika "podkręcania" piłki w zależności od miejsca uderzenia w paletkę.

### ⚡ System Power-Up (Bonusy)
Każdy zniszczony klocek ma **33% szans** na upuszczenie bonusu:
1.  🔵 **MultiBall:** Dodaje nową piłkę do gry (kluczowe dla przetrwania).
2.  🟢 **Expand Paddle:** Poszerza paletkę gracza, ułatwiając odbijanie.
3.  🟡 **Slow Ball:** Spowalnia wszystkie piłki na planszy o 30% (taktyczne spowolnienie).

### 💾 Zapis i Wyniki (Persistence)
* **Save/Load System:** Pełna serializacja stanu gry do pliku tekstowego. Wciśnięcie **F5** zapisuje wynik, poziom, życia, pozycje piłek i stan klocków.
* **Leaderboard (Tablica Wyników):** Po przegranej gracz wpisuje swój 3-literowy nick. Gra przechowuje 10 najlepszych wyników w pliku `highscores.txt`.

## 📦 Jak uruchomić grę?

###  Metoda 1: Szybkie Uruchomienie (Zalecane)
Nie musisz posiadać kompilatora ani Visual Studio. Przygotowana została gotowa paczka z grą.

1.  Przejdź do zakładki **[Releases](../../releases)** po prawej stronie.
2.  Pobierz najnowszy plik `.zip` (np. `Arkanoid_v1.0.0.zip`).
3.  Wypakuj archiwum w dowolnym miejscu.
4.  Uruchom plik **`ARKANOID.exe`**.
    * *Gra zawiera już wszystkie wymagane pliki DLL oraz czcionkę.*

---

###  Metoda 2: Kompilacja z Kodu Źródłowego
Jeśli chcesz edytować kod lub zbudować projekt samodzielnie:

**Wymagania:**
* Visual Studio 2022 (lub inny kompilator C++17).
* Biblioteka **SFML 3.0.0** skonfigurowana w środowisku.

**Instrukcja:**
1.  Sklonuj to repozytorium.
2.  Otwórz plik `ARKANOID.sln` w Visual Studio.
3.  Ustaw konfigurację na **Release** / **x64**.
4.  Skompiluj rozwiązanie (Build Solution).
5.  Upewnij się, że w folderze z plikiem `.exe` znajdują się pliki DLL SFML oraz `arial.ttf`.

## 🕹️ Sterowanie

| Klawisz | Akcja |
| :--- | :--- |
| **Strzałki (Lewo/Prawo)** | Poruszanie paletką / Nawigacja w menu |
| **Spacja / Enter** | Wybór w menu / Start gry |
| **F5** | **Zapisz grę (Save Game)** |
| **R** | Szybki restart (w trakcie gry lub po przegranej) |
| **ESC** | Wyjście do Menu / Pauza / Zamknięcie gry |

---
*Autor: Kamil Karpowicz | 2025*
