#ifndef DASHBOARDLOGIK_H
#define DASHBOARDLOGIK_H

#include <string>
#include <vector>
#include <chrono>
#include "HaushaltsplanerEngine.h" // Wir brauchen die Strukturen aus deiner Kern-Engine

// Die Struktur für das unkaputtbare Zustandsregister
struct WohnungsZustand {
    std::string name;
    bool istErfüllt; // z.B. "Küche_Müll_voll" -> true oder false
};

class DashboardLogik {
private:
    int tagesZeitBudgetPunkte;  // Unser Burnout-Schutz (1 Punkt = 5 Min)
    int aktuelleXP;             // Gamification: Deine gesammelten Erfahrungspunkte
    int aktuelleStreak;         // Gamification: Deine Erfolgsserie in Tagen

    std::vector<WohnungsZustand> zustandsRegister; // Das schützende Wörterbuch
    std::vector<EngineAufgabe> tagesAufgaben;     // Die gefilterte Liste für HEUTE

public:
    // Konstruktor: Startet das System mit 0 XP und 0 Streak
    DashboardLogik();

    // Zahnrad 1: Zeit-Budget setzen (Eingabe in Minuten wird in Punkte umgerechnet)
    void setZeitBudgetMinuten(int minuten);
    int getZeitBudgetPunkte() const;

    // Zahnrad 2: Interaktives Zustands-Register verwalten
    void initialisiereStandardZustände();
    void ändereZustand(const std::string& zustandName, bool status);
    const std::vector<WohnungsZustand>& getZustandsRegister() const;

    // Zahnrad 3: Der Smarte Sortierer (Filtert nach Bedingungen, Budget und Sonntagsruhe)
    void berechneTagesDashboard(const std::vector<EngineRaum>& alleRäume);
    const std::vector<EngineAufgabe>& getTagesAufgaben() const;

    // Zahnrad 4: Belohnungs-Logik
    void aufgabeErledigtXP(int punkte);
    int getXP() const;
    int getStreak() const;
};

#endif // DASHBOARDLOGIK_H
