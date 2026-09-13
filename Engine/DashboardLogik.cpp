#include "DashboardLogik.h"
#include <algorithm>
#include <iostream>
#include <chrono>

DashboardLogik::DashboardLogik() {
    tagesZeitBudgetPunkte = 0;
    aktuelleXP = 0;
    aktuelleStreak = 0;
}

void DashboardLogik::setZeitBudgetMinuten(int minuten) {
    tagesZeitBudgetPunkte = minuten / 5;
}

int DashboardLogik::getZeitBudgetPunkte() const {
    return tagesZeitBudgetPunkte;
}

// GEÄNDERT & REPARIERT: Der schlaue Nachrück-Sortierer mit präzisem Sonntagsfilter
void DashboardLogik::berechneTagesDashboard(const std::vector<EngineRaum>& alleRäume) {
    tagesAufgaben.clear();
    int verbrauchtePunkte = 0;

    // Automatischer Wochentags-Abruf über Chrono vom PC
    auto jetzt = std::chrono::system_clock::now();
    std::time_t zeitT = std::chrono::system_clock::to_time_t(jetzt);
    std::tm* aktuellesDatum = std::localtime(&zeitT);
    bool esIstSonntag = (aktuellesDatum->tm_wday == 0);

    for (size_t rIdx = 0; rIdx < alleRäume.size(); ++rIdx) {
        for (size_t aIdx = 0; aIdx < alleRäume[rIdx].aufgaben.size(); ++aIdx) {
            const auto& aufgabe = alleRäume[rIdx].aufgaben[aIdx];

            if (aufgabe.status != AufgabenStatus::BEREIT) continue;

            // CHANGE 4: Präziser Sonntagsfilter! Fegen erlaubt, nur Saugen und Bohren blockiert!
            if (esIstSonntag) {
                std::string nameKopie = aufgabe.name;
                std::transform(nameKopie.begin(), nameKopie.end(), nameKopie.begin(), ::tolower);
                if (nameKopie.find("saugen") != std::string::npos || nameKopie.find("bohren") != std::string::npos) {
                    continue;
                }
            }

            // CHANGE 2: Wenn es ins Budget passt, packen wir es auf die Tagesliste
            if (verbrauchtePunkte + aufgabe.punkte <= tagesZeitBudgetPunkte) {
                EngineAufgabe DashboardAnzeigeKopie = aufgabe;
                // Wir "schmuggeln" den Raumnamen in die Anzeige, damit man sieht, wo gearbeitet werden muss
                DashboardAnzeigeKopie.name = aufgabe.name + " (" + alleRäume[rIdx].name + ")";
                tagesAufgaben.push_back(DashboardAnzeigeKopie);
                verbrauchtePunkte += aufgabe.punkte;
            }
        }
    }
}

const std::vector<EngineAufgabe>& DashboardLogik::getTagesAufgaben() const { return tagesAufgaben; }
void DashboardLogik::aufgabeErledigtXP(int punkte) { aktuelleXP += (punkte * 10); }
int DashboardLogik::getXP() const { return aktuelleXP; }
int DashboardLogik::getStreak() const { return aktuelleStreak; }
