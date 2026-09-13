#ifndef DASHBOARDUI_H
#define DASHBOARDUI_H

#include "../Engine/DashboardLogik.h"
#include "../Engine/HaushaltsplanerEngine.h"

class DashboardUI {
public:
    void frageZeitBudgetAb(DashboardLogik& logik);
    void zeigeZustandsAnpassung(DashboardLogik& logik);

    // NEU: Der interaktive Initialisierungs-Check beim Öffnen des Dashboards
    void fuehreWohnungsCheckAus(HaushaltsplanerEngine& engine);

    void zeigeTäglichesDashboard(DashboardLogik& logik, const std::vector<EngineRaum>& alleRäume);
};

#endif // DASHBOARDUI_H
