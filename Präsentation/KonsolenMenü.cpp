#include "Haushaltsplaner.h"
#include <iostream>
#include <string>
#include <limits>

void HaushaltsplanerManager::zeigeWohnung(const HaushaltsplanerEngine& engine) {
    const auto& raeume = engine.getRaeume();
    if (raeume.empty()) {
        std::cout << "\n[!] Deine Wohnung hat noch keine Räume.\n";
        return;
    }

    std::cout << "\n=== DEINE WOHNUNG ===" << std::endl;
    for (size_t i = 0; i < raeume.size(); ++i) {
        std::cout << "[" << i << "] " << raeume[i].name << "\n";
    }
    std::cout << "=====================" << std::endl;
}

void HaushaltsplanerManager::öffneRaumMenü(int raumIndex, HaushaltsplanerEngine& engine) {
    const auto& raeume = engine.getRaeume();
    if (raumIndex < 0 || raumIndex >= raeume.size()) {
        std::cout << "\n[!] Ungültige Raum-Nummer.\n";
        return;
    }

    while (true) {
        const auto& aktuellerRaum = raeume[raumIndex];
        std::cout << "\n--- RAUM-ÜBERSICHT: " << aktuellerRaum.name << " ---" << std::endl;
        std::cout << "Hier sind alle registrierten Aufgaben:\n";

        if (aktuellerRaum.aufgaben.empty()) {
            std::cout << "  (Keine Aufgaben in diesem Raum)\n";
        } else {
            for (size_t i = 0; i < aktuellerRaum.aufgaben.size(); ++i) {
                const auto& aufgabe = aktuellerRaum.aufgaben[i];
                std::string statusStr;

                switch (aufgabe.status) {
                    case AufgabenStatus::BEREIT:   statusStr = "[Offen]"; break;
                    case AufgabenStatus::GEBLOCKT: statusStr = "[🔒 Blockiert]"; break;
                    case AufgabenStatus::ERLEDIGT: statusStr = "[✔ Erledigt]"; break;
                    case AufgabenStatus::GESKIPPT:  statusStr = "[⏭ Geskippt]"; break;
                }
                std::cout << "  [" << i << "] " << statusStr << " " << aufgabe.name << " (" << aufgabe.punkte << " Punkte)\n";
            }
        }

        std::cout << "\nOptionen:\n";
        std::cout << "-1. Zurück zur Wohnungsübersicht (Einen Schritt zurück)\n";
        std::cout << "99. DIREKT ZURÜCK ZUM HAUPTMENÜ\n";
        std::cout << "Auswahl: ";
        int aktion = 0;

        if (!(std::cin >> aktion)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[!] Bitte gib eine Zahl ein.\n";
            continue;
        }

        if (aktion == 99) {
            std::cin.clear();
            throw 99; // Wirft das Signal für das Hauptmenü
        }
        if (aktion == -1) {
            break; // Beendet NUR diese Schleife -> geht zurück zur Wohnungsübersicht
        }

        std::cout << "\n[Info] In dieser Ansicht können Aufgaben nur eingesehen werden. Nutze das Dashboard zum Arbeiten!\n";
    }
}
