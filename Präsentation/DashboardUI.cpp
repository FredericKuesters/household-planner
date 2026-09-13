#include "DashboardUI.h"
#include <iostream>
#include <limits>
#include <algorithm>

void DashboardUI::frageZeitBudgetAb(DashboardLogik& logik) {
    int minuten = 0;
    std::cout << "\n=============================================" << std::endl;
    std::cout << "     WILLKOMMEN IN DEINER RECHEN-ZENTRALE    " << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Wie viele Minuten hast du heute für den Haushalt? ";

    while (!(std::cin >> minuten) || minuten < 0) {
        std::cout << "[!] Bitte gib eine gültige Minutenzahl ein: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    logik.setZeitBudgetMinuten(minuten);
}

void DashboardUI::zeigeZustandsAnpassung(DashboardLogik& logik) {}

void DashboardUI::fuehreWohnungsCheckAus(HaushaltsplanerEngine& engine) {
    if (!engine.getAktivesRegister().empty()) {
        return;
    }

    std::vector<std::string> moegliche = engine.getAlleMoeglichenZustaende();
    if (moegliche.empty()) return;

    std::cout << "\n=======================================================" << std::endl;
    std::cout << "   ERSTMALIGER WOHNUNGS-CHECK (Status-Initialisierung) " << std::endl;
    std::cout << "   Bitte sag dem System kurz, was heute zutrifft:       " << std::endl;
    std::cout << "=======================================================" << std::endl;

    for (const auto& zustand : moegliche) {
        std::cout << "Trifft '" << zustand << "' aktuell bei dir zu? (1 = Ja, 0 = Nein): ";
        int antwort = 0;

        // KORRIGIERT: Prüft sauber, ob die Eingabe eine Zahl und ENTWEDER 0 ODER 1 ist
        while (!(std::cin >> antwort) || (antwort != 0 && antwort != 1)) {
            std::cout << "[!] Bitte nur 1 (Ja) oder 0 (Nein) eingeben: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (antwort == 1) {
            engine.setZustand(zustand, true);
        }
    }

    std::cout << "[Info] Wohnung erfolgreich initialisiert!\n";
}

void DashboardUI::zeigeTäglichesDashboard(DashboardLogik& logik, const std::vector<EngineRaum>& alleRäume) {
    std::vector<EngineRaum>& veraenderbareRaeume = const_cast<std::vector<EngineRaum>&>(alleRäume);

    while (true) {
        logik.berechneTagesDashboard(veraenderbareRaeume);
        const auto& aufgaben = logik.getTagesAufgaben();

        std::cout << "\n=======================================================" << std::endl;
        std::cout << "   DEIN TÄGLICHES DASHBOARD  |  Streak: " << logik.getStreak() << " Tage 🔥" << std::endl;
        std::cout << "   Gesammelte Erfahrung:     " << logik.getXP() << " XP ⭐" << std::endl;
        std::cout << "   Verbleibendes Budget:     " << logik.getZeitBudgetPunkte() << " Punkte (" << logik.getZeitBudgetPunkte() * 5 << " Min)" << std::endl;
        std::cout << "=======================================================" << std::endl;

        if (aufgaben.empty()) {
            std::cout << "\n 🎉 Keine Aufgaben mehr offen oder Zeitbudget aufgebraucht.\n\n";
            std::cout << " Drücke die Enter-Taste um zurückzukehren... ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }

        std::cout << "Deine Aufgaben für heute:\n";
        for (size_t i = 0; i < aufgaben.size(); ++i) {
            std::cout << "  [" << i << "] " << aufgaben[i].name << " (" << aufgaben[i].punkte << " Punkte)\n";
        }
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Option wählen:\n";
        std::cout << "1. Eine Aufgabe als ERLEDIGT markieren\n";
        std::cout << "2. Eine Aufgabe ALS NICHT NÖTIG wegschieben (Skippen)\n";
        std::cout << "3. Dashboard VERLASSEN (Zurück zum Hauptmenü OHNE SPEICHERN)\n"; // DEIN NEUER WUNSCH!
        std::cout << "Auswahl: ";
        int aktion = 0; std::cin >> aktion;

        if (aktion == 3 || aktion < 1 || aktion > 3) {
            std::cout << "\n[Dashboard] Verlassen ohne Speichern.\n";
            break; // Bricht das Dashboard sofort ab, ohne die zustand.txt zu sichern!
        }

        std::cout << "Nummer der Aufgabe eingeben: ";
        size_t wahl = 0; std::cin >> wahl;

        if (wahl >= aufgaben.size()) {
            std::cout << "[!] Ungültige Nummer.\n";
            continue;
        }

        std::string gesuchterName = aufgaben[wahl].name;
        size_t klammer = gesuchterName.find(" (");
        if (klammer != std::string::npos) {
            gesuchterName = gesuchterName.substr(0, klammer);
        }

        bool abgehakt = false;
        for (size_t r = 0; r < veraenderbareRaeume.size(); ++r) {
            for (size_t a = 0; a < veraenderbareRaeume[r].aufgaben.size(); ++a) {
                if (veraenderbareRaeume[r].aufgaben[a].name == gesuchterName) {
                    if (aktion == 1) {
                        veraenderbareRaeume[r].aufgaben[a].status = AufgabenStatus::ERLEDIGT;
                        logik.aufgabeErledigtXP(veraenderbareRaeume[r].aufgaben[a].punkte);
                        int neuesBudget = logik.getZeitBudgetPunkte() - veraenderbareRaeume[r].aufgaben[a].punkte;
                        logik.setZeitBudgetMinuten(neuesBudget * 5);
                        std::cout << "\n⭐ +" << veraenderbareRaeume[r].aufgaben[a].punkte * 10 << " XP verdient!\n";
                    }
                    else if (aktion == 2) {
                        veraenderbareRaeume[r].aufgaben[a].status = AufgabenStatus::GESKIPPT;
                        std::cout << "\n[Info] Aufgabe für heute ausgeblendet.\n";
                    }
                    abgehakt = true; break;
                }
            }
            if (abgehakt) break;
        }
    }
}
