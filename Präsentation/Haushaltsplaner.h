#ifndef HAUSHALTSPLANER_HAUSHALTSPLANER_H
#define HAUSHALTSPLANER_HAUSHALTSPLANER_H

#include <string>
#include <vector>

// Wir müssen den Engine-Header hier einbinden, damit diese Klasse weiß, was eine "HaushaltsplanerEngine" ist
#include "../Engine/HaushaltsplanerEngine.h"

// Die Klasse für unsere Präsentations-Ebene (Das Konsolen-Menü)
class HaushaltsplanerManager {
public:
    // Funktion 1: Zeigt die Räume an. Sie bekommt das Datengehirn als "const" (nur zum Lesen) übergeben.
    void zeigeWohnung(const HaushaltsplanerEngine& engine);

    // Funktion 2: Öffnet das interaktive Untermenü für einen Raum. Sie darf die Engine verändern (Status auf ERLEDIGT/GESKIPPT setzen).
    void öffneRaumMenü(int raumIndex, HaushaltsplanerEngine& engine);
    // NEU: Benennt einen Zustand im aktiven Register und in allen verknüpften Aufgaben um
    void benenneZustandUm(const std::string& alterName, const std::string& neuerName);

};

#endif //HAUSHALTSPLANER_HAUSHALTSPLANER_H
