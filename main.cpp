#include <iostream>
#include <string>
#include <limits>
#include <filesystem>
#include "Engine/HaushaltsplanerEngine.h"
#include "Engine/DashboardLogik.h"
#include "Präsentation/Haushaltsplaner.h"
#include "Präsentation/DashboardUI.h"

int main() {
    // Sicherheitsgurt für den Datenordner
    if (!std::filesystem::exists("daten")) {
        std::filesystem::create_directory("daten");
    }

    HaushaltsplanerEngine engine;
    DashboardLogik dashboardLogik;
    HaushaltsplanerManager raumMenueManager;
    DashboardUI dashboardUI;

    const std::string aufgabenDatei = "daten/aufgaben.txt";
    const std::string zustandDatei = "daten/zustand.txt";

    // Daten sauber laden
    engine.ladeAufgabenAusDatei(aufgabenDatei);
    engine.ladeZustaendeAusDatei(zustandDatei);

    int choice = 0;

    while (true) {
        std::cout << "=== INTERAKTIVER HAUSHALTSPLANER ===" << std::endl;
        std::cout << "1. TÄGLICHES DASHBOARD ÖFFNEN" << std::endl;
        std::cout << "2. Wohnung verwalten (Räume öffnen)" << std::endl;
        std::cout << "3. Neue Aufgabe / Raum hinzufügen" << std::endl;
        std::cout << "4. ZUSTÄNDE VERWALTEN" << std::endl;
        std::cout << "5. Programm beenden" << std::endl;
        std::cout << "Auswahl eingeben: ";

        if (!(std::cin >> choice)) {
            std::cout << "\n[!] Bitte gib eine Zahl ein!\n" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 5) {
            engine.speichereAufgabenInDatei(aufgabenDatei);
            engine.speichereZustaendeInDatei(zustandDatei);
            std::cout << "Fortschritt gesichert. Tschüss!" << std::endl;
            break;
        }

        if (choice == 1) {
            dashboardUI.fuehreWohnungsCheckAus(engine);
            dashboardUI.frageZeitBudgetAb(dashboardLogik);
            dashboardUI.zeigeTäglichesDashboard(dashboardLogik, engine.getRaeume());
        } else if (choice == 2) {
            try {
                // Diese innere Schleife hält den Nutzer in der Wohnungsübersicht
                while (true) {
                    raumMenueManager.zeigeWohnung(engine);
                    if (engine.getRaeume().empty()) {
                        break;
                    }

                    int raumWahl = 0;
                    std::cout << "Welchen Raum öffnen? (Nummer eingeben, oder '-1' für zurück zum Hauptmenü): ";

                    if (!(std::cin >> raumWahl)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[!] Ungültige Eingabe.\n";
                        continue;
                    }

                    // -1 bricht nur die innere Schleife ab -> zurück ins Hauptmenü
                    if (raumWahl == -1) {
                        break;
                    }

                    if (raumWahl >= 0 && static_cast<size_t>(raumWahl) < engine.getRaeume().size()) {
                        // Wenn das Raummenü per -1 (break) schließt, bleibt man in dieser Schleife!
                        raumMenueManager.öffneRaumMenü(raumWahl, engine);
                    } else {
                        std::cout << "[!] Ungültige Raumnummer.\n";
                    }
                }
            } catch (int signal) {
                // Wenn du tief im Raum-Menü 99 drückst, fliegst du per Signal hierher
                // und landest sofort komplett draußen im Hauptmenü!
                if (signal == 99) {
                    std::cout << "\n[Shortcut] Direkt zurück ins Hauptmenü gesprungen.\n";
                }
            }
        } else if (choice == 3) {
            int raumTypWahl;
            std::string rName, aName, vorbed, eff;
            int punkte;

            std::cout << "\n--- NEUE AUFGABE EINPFLEGEN ---" << std::endl;
            std::cout << "In welchen Raum gehört die Aufgabe?\n";
            std::cout << "1: Küche, 2: Badezimmer, 3: Hauptzimmer, 4: Wohnzimmer, 5: Flur, 6: Terrasse\n";
            std::cout << "Auswahl: ";
            while (!(std::cin >> raumTypWahl) || raumTypWahl < 1 || raumTypWahl > 6) {
                std::cout << "[!] Bitte Zahl von 1-6 eingeben: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (raumTypWahl == 1) rName = "Küche";
            else if (raumTypWahl == 2) rName = "Badezimmer";
            else if (raumTypWahl == 3) rName = "Hauptzimmer";
            else if (raumTypWahl == 4) rName = "Wohnzimmer";
            else if (raumTypWahl == 5) rName = "Flur";
            else if (raumTypWahl == 6) rName = "Terrasse";

            std::cout << "\n-------------------------------------------------------" << std::endl;
            std::cout << "[Check] Du bearbeitest gerade den Raum: " << rName << std::endl;
            std::cout << "Folgende Aufgaben existieren hier bereits:" << std::endl;
            bool raumGefunden = false;
            for (const auto &raum: engine.getRaeume()) {
                if (raum.name == rName) {
                    raumGefunden = true;
                    if (raum.aufgaben.empty()) {
                        std::cout << "  (Noch keine Aufgaben in diesem Raum)\n";
                    } else {
                        for (const auto &aufgabe: raum.aufgaben) {
                            std::cout << "  - " << aufgabe.name << "\n";
                        }
                    }
                }
            }
            if (!raumGefunden) std::cout << "  (Raum wird komplett neu angelegt)\n";
            std::cout << "-------------------------------------------------------\n" << std::endl;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Wie heißt die neue Aufgabe? (Keine Duplikate erstellen!): ";
            std::getline(std::cin, aName);

            std::cout << "Zeitaufwand in Punkten (1 Punkt = 5 Min): ";
            while (!(std::cin >> punkte) || punkte < 1) {
                std::cout << "[!] Bitte eine Zahl eingeben: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "\nWelche Voraussetzung hat die Aufgabe?\n";
            std::cout <<
                    "  0. Keine Voraussetzung\n  1. Küche: Müll voll\n  2. Küche: Spülmaschine voll\n  3. Küche: Spülmaschine sauber\n  4. Hauptzimmer: Boden frei\n  5. Neuen Zustand frei eintippen\nAuswahl: ";
            int vWahl;
            std::cin >> vWahl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (vWahl == 1) vorbed = "Küche: Müll voll";
            else if (vWahl == 2) vorbed = "Küche: Spülmaschine voll";
            else if (vWahl == 3) vorbed = "Küche: Spülmaschine sauber";
            else if (vWahl == 4) vorbed = "Hauptzimmer: Boden frei";
            else if (vWahl == 5) {
                std::cout << "Neuen Zustand eingeben: ";
                std::getline(std::cin, vorbed);
            } else vorbed = "";

            std::cout << "\nWelche Auswirkung hat die Aufgabe?\n";
            std::cout <<
                    "  0. Keine Auswirkung\n  1. Küche: Müll voll\n  2. Küche: Spülmaschine voll\n  3. Küche: Spülmaschine sauber\n  4. Hauptzimmer: Boden frei\n  5. Neuen Zustand frei eintippen\nAuswahl: ";
            int eWahl;
            std::cin >> eWahl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (eWahl == 1) eff = "Küche: Müll voll";
            else if (eWahl == 2) eff = "Küche: Spülmaschine voll";
            else if (eWahl == 3) eff = "Küche: Spülmaschine sauber";
            else if (eWahl == 4) eff = "Hauptzimmer: Boden frei";
            else if (eWahl == 5) {
                std::cout << "Neuen Zustand eingeben: ";
                std::getline(std::cin, eff);
            } else eff = "";

            engine.fuegeAufgabeHinzu(rName, aName, punkte, vorbed, eff);
            engine.speichereAufgabenInDatei(aufgabenDatei);
            std::cout << "\n[✔] Aufgabe erfolgreich gespeichert!\n" << std::endl;
        } else if (choice == 4) {
            while (true) {
                std::cout << "\n=== ZUSTANDS-REGISTER VERWALTEN ===" << std::endl;
                std::vector<std::string> moegliche = engine.getAlleMoeglichenZustaende();

                for (size_t i = 0; i < moegliche.size(); ++i) {
                    std::string statusStr = engine.istZustandAktiv(moegliche[i]) ? "[JA / VOLL]" : "[NEIN / LEER]";
                    std::cout << "  [" << i << "] " << moegliche[i] << ": " << statusStr << "\n";
                }
                std::cout << "  [-1] Zurück zum Hauptmenü\n";
                std::cout << "Auswahl eingeben: ";
                int zWahl = 0;

                if (!(std::cin >> zWahl)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[!] Bitte gib eine Zahl ein.\n";
                    continue;
                }

                if (zWahl == -1) break;

                if (zWahl >= 0 && static_cast<size_t>(zWahl) < moegliche.size()) {
                    std::string gewaehlterZustand = moegliche[zWahl];

                    std::cout << "\nAusgewählter Zustand: " << gewaehlterZustand << std::endl;
                    std::cout << "1. Status ändern (JA <-> NEIN)\n";
                    std::cout << "2. Zustand korrigieren / umbenennen\n";
                    std::cout << "3. Abbrechen\n";
                    std::cout << "Auswahl: ";
                    int subAktion = 0;
                    std::cin >> subAktion;
                    if (subAktion == 1) {
                        bool aktuellerWert = engine.istZustandAktiv(gewaehlterZustand);
                        engine.setZustand(gewaehlterZustand, !aktuellerWert);
                        engine.speichereZustaendeInDatei(zustandDatei);
                        std::cout << "\n[✔] Status erfolgreich umgeschaltet!\n";
                    } else if (subAktion == 2) {std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::string neuerName;
                        std::cout << "Gib den korrigierten Namen für diesen Zustand ein: ";
                        std::getline(std::cin, neuerName);
                        if (!neuerName.empty()) {
                            engine.benenneZustandUm(gewaehlterZustand, neuerName);
                            engine.speichereAufgabenInDatei(aufgabenDatei);
                            engine.speichereZustaendeInDatei(zustandDatei);
                            std::cout << "\n[✔] Zustand erfolgreich umbenannt!\n";
                        }
                    }
                } else { std::cout << "[!] Ungültige Nummer.\n"; }
            }
        } else { std::cout << "\n[!] Ungültige Auswahl, versuche es nochmal.\n" << std::endl; }
    }
    return 0;
}
