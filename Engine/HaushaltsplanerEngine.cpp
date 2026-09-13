#include "HaushaltsplanerEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Funktion 1: Aufgaben aus der Textdatei einlesen
bool HaushaltsplanerEngine::ladeAufgabenAusDatei(const std::string& dateiName) {
    std::ifstream datei(dateiName);
    if (!datei.is_open()) {
        // Mindestwohnung mit sauberen Raum-Präfixen für maximale Synergie!
        fuegeAufgabeHinzu("Küche", "Küchenmüll leeren", 1, "", "Küche: Müll voll");
        fuegeAufgabeHinzu("Küche", "Spülmaschine einräumen", 2, "", "Küche: Spülmaschine voll");
        fuegeAufgabeHinzu("Küche", "Spülmaschine anschmeißen", 1, "Küche: Spülmaschine voll", "Küche: Spülmaschine sauber");
        fuegeAufgabeHinzu("Küche", "Spülmaschine ausräumen", 2, "Küche: Spülmaschine sauber", "");
        fuegeAufgabeHinzu("Badezimmer", "Toilette bürsten", 1, "", "");
        fuegeAufgabeHinzu("Hauptzimmer", "Sachen aufräumen", 2, "", "Hauptzimmer: Boden frei");
        fuegeAufgabeHinzu("Hauptzimmer", "Boden saugen", 3, "Hauptzimmer: Boden frei", "");

        speichereAufgabenInDatei(dateiName);
        datei.open(dateiName);
    }

    raeume.clear();
    std::string zeile;
    while (std::getline(datei, zeile)) {
        if (zeile.empty()) continue;
        std::stringstream ss(zeile);
        std::string raumName, aufgabenName, punkteStr, voraussetzung, auswirkung;

        std::getline(ss, raumName, '|');
        std::getline(ss, aufgabenName, '|');
        std::getline(ss, punkteStr, '|');
        std::getline(ss, voraussetzung, '|');
        std::getline(ss, auswirkung, '|');

        int punkte = 0;
        if (!punkteStr.empty()) punkte = std::stoi(punkteStr);
        EngineAufgabe neueAufgabe = {aufgabenName, AufgabenStatus::BEREIT, punkte, voraussetzung, auswirkung};

        auto it = std::find_if(raeume.begin(), raeume.end(), [&](const EngineRaum& r) { return r.name == raumName; });
        if (it != raeume.end()) {
            it->aufgaben.push_back(neueAufgabe);
        } else {
            EngineRaum neuerRaum;
            neuerRaum.name = raumName;
            neuerRaum.aufgaben.push_back(neueAufgabe);
            raeume.push_back(neuerRaum);
        }
    }
    datei.close();
    aktualisiereAufgabenStatus();
    return true;
}

// Funktion 2: Aufgaben auf Festplatte sichern
bool HaushaltsplanerEngine::speichereAufgabenInDatei(const std::string& dateiName) {
    std::ofstream datei(dateiName);
    if (!datei.is_open()) return false;

    for (const auto& raum : raeume) {
        for (const auto& aufgabe : raum.aufgaben) {
            datei << raum.name << "|" << aufgabe.name << "|" << aufgabe.punkte << "|"
                  << aufgabe.voraussetzung << "|" << aufgabe.auswirkung << "\n";
        }
    }
    datei.close();
    return true;
}

// Funktion 3: Zustände aus Datei laden
bool HaushaltsplanerEngine::ladeZustaendeAusDatei(const std::string& dateiName) {
    std::ifstream datei(dateiName);
    if (!datei.is_open()) return false;

    aktivesZustandsRegister.clear();
    std::string zeile;
    while (std::getline(datei, zeile)) {
        if (zeile.empty()) continue;
        std::stringstream ss(zeile);
        std::string zustandName, wertStr;
        std::getline(ss, zustandName, '|');
        std::getline(ss, wertStr, '|');
        if (wertStr == "true") {
            aktivesZustandsRegister.push_back(zustandName);
        }
    }
    datei.close();
    aktualisiereAufgabenStatus();
    return true;
}

// Funktion 4: Zustände auf Festplatte sichern
bool HaushaltsplanerEngine::speichereZustaendeInDatei(const std::string& dateiName) {
    std::ofstream datei(dateiName);
    if (!datei.is_open()) return false;

    for (const auto& zustand : aktivesZustandsRegister) {
        datei << zustand << "|true\n";
    }
    datei.close();
    return true;
}

// Funktion 5: Logischen Status berechnen (Ketten-Prüfung)
void HaushaltsplanerEngine::aktualisiereAufgabenStatus() {
    for (auto& raum : raeume) {
        for (auto& aufgabe : raum.aufgaben) {
            if (aufgabe.status == AufgabenStatus::ERLEDIGT || aufgabe.status == AufgabenStatus::GESKIPPT) {
                continue;
            }
            if (aufgabe.voraussetzung.empty()) {
                aufgabe.status = AufgabenStatus::BEREIT;
                continue;
            }
            auto it = std::find(aktivesZustandsRegister.begin(), aktivesZustandsRegister.end(), aufgabe.voraussetzung);
            if (it != aktivesZustandsRegister.end()) {
                aufgabe.status = AufgabenStatus::BEREIT;
            } else {
                aufgabe.status = AufgabenStatus::GEBLOCKT;
            }
        }
    }
}

// Funktion 6: Interaktives Erledigen
void HaushaltsplanerEngine::aufgabeErledigen(int raumIndex, int aufgabenIndex) {
    if (raumIndex >= 0 && raumIndex < raeume.size()) {
        auto& raum = raeume[raumIndex];
        if (aufgabenIndex >= 0 && aufgabenIndex < raum.aufgaben.size()) {
            auto& aufgabe = raum.aufgaben[aufgabenIndex];
            aufgabe.status = AufgabenStatus::ERLEDIGT;
            if (!aufgabe.auswirkung.empty()) {
                setZustand(aufgabe.auswirkung, true);
            }
            aktualisiereAufgabenStatus();
        }
    }
}

// Funktion 7: Interaktives Skippen
void HaushaltsplanerEngine::aufgabeSkippen(int raumIndex, int aufgabenIndex) {
    if (raumIndex >= 0 && raumIndex < raeume.size()) {
        if (aufgabenIndex >= 0 && aufgabenIndex < raeume[raumIndex].aufgaben.size()) {
            raeume[raumIndex].aufgaben[aufgabenIndex].status = AufgabenStatus::GESKIPPT;
            aktualisiereAufgabenStatus();
        }
    }
}

// Funktion 8: Dynamisches Hinzufügen einer Aufgabe (BUGFIX!)
void HaushaltsplanerEngine::fuegeAufgabeHinzu(const std::string& raumName, const std::string& aufgabenName, int punkte, const std::string& vorbedienung, const std::string& effekt) {
    EngineAufgabe neueAufgabe = {aufgabenName, AufgabenStatus::BEREIT, punkte, vorbedienung, effekt};
    auto it = std::find_if(raeume.begin(), raeume.end(), [&](const EngineRaum& r) { return r.name == raumName; });

    if (it != raeume.end()) {
        it->aufgaben.push_back(neueAufgabe);
    } else {
        EngineRaum neuerRaum;
        neuerRaum.name = raumName;
        neuerRaum.aufgaben.push_back(neueAufgabe);
        raeume.push_back(neuerRaum);
    }
    aktualisiereAufgabenStatus();
}

// Hilfsfunktionen für das Register
void HaushaltsplanerEngine::setZustand(const std::string& zustand, bool wert) {
    auto it = std::find(aktivesZustandsRegister.begin(), aktivesZustandsRegister.end(), zustand);
    if (wert) {
        if (it == aktivesZustandsRegister.end()) aktivesZustandsRegister.push_back(zustand);
    } else {
        if (it != aktivesZustandsRegister.end()) aktivesZustandsRegister.erase(it);
    }
    aktualisiereAufgabenStatus();
}

bool HaushaltsplanerEngine::istZustandAktiv(const std::string& zustand) const {
    auto it = std::find(aktivesZustandsRegister.begin(), aktivesZustandsRegister.end(), zustand);
    return it != aktivesZustandsRegister.end();
}

// Schalterliste sortiert mit Präfixen ausgeben
std::vector<std::string> HaushaltsplanerEngine::getAlleMoeglichenZustaende() const {
    std::vector<std::string> alleZustaende;
    alleZustaende.push_back("Küche: Müll voll");
    alleZustaende.push_back("Küche: Spülmaschine voll");
    alleZustaende.push_back("Küche: Spülmaschine sauber");
    alleZustaende.push_back("Hauptzimmer: Boden frei");

    for (const auto& raum : raeume) {
        for (const auto& aufgabe : raum.aufgaben) {
            if (!aufgabe.voraussetzung.empty() && std::find(alleZustaende.begin(), alleZustaende.end(), aufgabe.voraussetzung) == alleZustaende.end()) {
                alleZustaende.push_back(aufgabe.voraussetzung);
            }
            if (!aufgabe.auswirkung.empty() && std::find(alleZustaende.begin(), alleZustaende.end(), aufgabe.auswirkung) == alleZustaende.end()) {
                alleZustaende.push_back(aufgabe.auswirkung);
            }
        }
    }
    return alleZustaende;
}

// Getter-Funktionen
std::vector<std::string>& HaushaltsplanerEngine::getAktivesRegister() { return aktivesZustandsRegister; }
const std::vector<EngineRaum>& HaushaltsplanerEngine::getRaeume() const { return raeume; }

void HaushaltsplanerEngine::benenneZustandUm(const std::string& alterName, const std::string& neuerName) {
    if (alterName.empty() || neuerName.empty()) return;

    // 1. Im aktiven Register umbenennen
    auto it = std::find(aktivesZustandsRegister.begin(), aktivesZustandsRegister.end(), alterName);
    if (it != aktivesZustandsRegister.end()) {
        *it = neuerName;
    }

    // 2. In allen Aufgaben bei Voraussetzungen und Auswirkungen anpassen, damit die Ketten heil bleiben!
    for (auto& raum : raeume) {
        for (auto& aufgabe : raum.aufgaben) {
            if (aufgabe.voraussetzung == alterName) {
                aufgabe.voraussetzung = neuerName;
            }
            if (aufgabe.auswirkung == alterName) {
                aufgabe.auswirkung = neuerName;
            }
        }
    }
    aktualisiereAufgabenStatus();
}
