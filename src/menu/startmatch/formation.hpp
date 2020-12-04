// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#ifndef _HPP_MENU_FORMATION
#define _HPP_MENU_FORMATION

#include "utils/gui2/windowmanager.hpp"

#include "utils/gui2/widgets/menu.hpp"
#include "utils/gui2/widgets/root.hpp"
#include "utils/gui2/widgets/grid.hpp"
#include "utils/gui2/widgets/button.hpp"
#include "utils/gui2/widgets/image.hpp"
#include "utils/gui2/widgets/slider.hpp"
#include "utils/gui2/widgets/iconselector.hpp"
#include "utils/gui2/widgets/pulldown.hpp"
#include "menu/widgets/planmap.hpp"
#include "menu/widgets/gameplansubmenu.hpp"
#include "utils/database.hpp"

using namespace blunted;

struct TacticsSliderFormation {
  int id;
  Gui2Slider* widget;
  std::string tacticName;
};

class FormationPage : public Gui2Page {

  public:
    FormationPage(Gui2WindowManager *windowManager, const Gui2PageData &pageData);
    virtual ~FormationPage();
    void GoLoadingMatchPage();
    Vector3 GetButtonColor(int id);
    void SaveLineup(int part);
    void LineupMenuOnClick(Gui2Button *button, int part);
    void LineupMenuOnFocus(Gui2Button *button, int part);
    void GoLineupMenu_one();
    void GoLineupMenu_two();
    void GoTacticsMenu_one();
    void GoTacticsMenu_two();
    void GoReady_one();
    void GoReady_two();
    void Deactivate(int part);
    void Reactivate(int part);
    void TacticsMenuOnChange(Gui2Slider *slider, int id, int part);
    void SaveTactics_one();
    virtual void ProcessWindowingEvent(WindowingEvent *event);

  protected:
    TeamData *teamData1;
    TeamData *teamData2;
    Gui2PlanMap *map_one;
    Gui2PlanMap *map_two;
    Gui2Button *buttonStart;
    Gui2Grid *grid_one;
    Gui2Grid *gridNav_one;
    Gui2Grid *grid_two;
    Gui2Grid *gridNav_two;
    Gui2Button *buttonLineup_one;
    Gui2Button *buttonTactics_one;
    Gui2Button *buttonReady_one;
    Gui2Button *buttonLineup_two;
    Gui2Button *buttonTactics_two;
    Gui2Button *buttonReady_two;
    GamePlanSubMenu *lineupMenu_one;
    GamePlanSubMenu *lineupMenu_two;
    GamePlanSubMenu *tacticsMenu_one;
    GamePlanSubMenu *tacticsMenu_two;
    std::vector<TacticsSliderFormation> tacticsSliders_one;
    std::vector<TacticsSliderFormation> tacticsSliders_two;
    Database *namedb;
};

#endif
