#ifndef HAUSHALTSPLANERENGINE_H
#define HAUSHALTSPLANERENGINE_H

#include <string>
#include <vector>

enum class AufgabenStatus {
    BEREIT,
    GEBLOCKT,
    ERLEDIGT,
    GESKIPPT
};

struct EngineAufgabe {
    std::string name;
    AufgabenStatus status;
    int punkte;
    std::string voraussetzung;
    std::string auswirkung;
};

struct EngineRaum {
    std::string name;
    std::vector<EngineAufgabe> aufgaben;
};

class HaushaltsplanerEngine {
private:
    std::vector<EngineRaum> raeume;
    std::vector<std::string> aktivesZustandsRegister;

public:
    bool ladeAufgabenAusDatei(const std::string& dateiName);
    bool speichereAufgabenInDatei(const std::string& dateiName);

    bool ladeZustaendeAusDatei(const std::string& dateiName);
    bool speichereZustaendeInDatei(const std::string& dateiName);

    void aktualisiereAufgabenStatus();
    void aufgabeErledigen(int raumIndex, int aufgabenIndex);
    void aufgabeSkippen(int raumIndex, int aufgabenIndex);
    void fuegeAufgabeHinzu(const std::string& raumName, const std::string& aufgabenName, int punkte, const std::string& vorbedienung, const std::string& effekt);

    std::vector<std::string>& getAktivesRegister();
    void setZustand(const std::string& zustand, bool wert);
    bool istZustandAktiv(const std::string& zustand) const;
    std::vector<std::string> getAlleMoeglichenZustaende() const;

    // HIER REIN: Die Deklaration steht jetzt sicher vor der schließenden Klammer!
    void benenneZustandUm(const std::string& alterName, const std::string& neuerName);

    const std::vector<EngineRaum>& getRaeume() const;
}; // <-- DIESE KLAMMER UND DAS SEMIKOLON WURDEN GEFORDEERT!

#endif // HAUSHALTSPLANERENGINE_H
