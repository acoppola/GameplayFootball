// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "planmap.hpp"

#include "utils/gui2/windowmanager.hpp"

#include "utils/gui2/widgets/image.hpp"

#include "../../gamedefines.hpp"

#include "../../data/teamdata.hpp"

namespace blunted {

  Gui2PlanMap::Gui2PlanMap(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, TeamData *teamData) : Gui2View(windowManager, name, x_percent, y_percent, width_percent, height_percent), teamData(teamData) {

    Gui2Image *bg = new Gui2Image(windowManager, "image_planmap_bg", 0, 0, width_percent, height_percent);
    bg->LoadImage("media/menu/planmap.png");
    this->AddView(bg);
    bg->Show();
    toRedraw = true;
    Redraw();
  }

  Gui2PlanMap::~Gui2PlanMap() {
  }

  void Gui2PlanMap::Process() {
    Gui2View::Process();
  }

  void Gui2PlanMap::SetForRedraw() {
    toRedraw = true;
  }

  void Gui2PlanMap::Redraw() {
    toRedraw = true;
    if(toRedraw){
      std::vector<Gui2View*> childrens = this->GetChildren();
      for (int i = 0; i < (signed int)childrens.size(); i++) {
        if (childrens.at(i)->GetName().substr(0, 6).compare("planma") == 0)
          this->RemoveView(childrens.at(i));
      }
      for (int i = 0; i < 11; i++) {
        Gui2PlanMapEntry *entry = 0;
        Vector3 pos = teamData->GetFormationEntry(i).databasePosition;
        if (teamData->GetFormationEntry(i).role != e_PlayerRole_GK) { pos.coords[0] *= 0.8f; pos.coords[0] += 0.1f; } // compress field players' depth
        float ex = (pos.coords[0] *  0.44 + 0.5) * width_percent - 1.5;
        float ey = (pos.coords[1] * -0.44 + 0.5) * height_percent - 1.5;
        int pOverall = teamData->GetPlayerData(i)->GetOverall();
        std::string playerName = teamData->GetPlayerData(i)->GetLastName();
        entry = new Gui2PlanMapEntry(windowManager, "planmap_player1_entry" + int_to_str(i), ex, ey, 3, 3, GetRoleName(teamData->GetFormationEntry(i).role), playerName, int_to_str(pOverall));
        this->AddView(entry);
        entry->Show();
      }
      toRedraw = false;
    }
  }


  // ENTRIES
    Gui2PlanMapEntry::Gui2PlanMapEntry(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, const std::string &roleName, const std::string &playerName, const std::string &overall) : Gui2View(windowManager, name, x_percent, y_percent, width_percent, height_percent) {

    roleNameCaption = new Gui2Caption(windowManager, roleName + "role", 0, 0, width_percent, height_percent, roleName);
    this->AddView(roleNameCaption);
    roleNameCaption->Show();

    playerNameCaption = new Gui2Caption(windowManager, playerName + "name", 0, 2, width_percent, height_percent - 1, playerName);
    this->AddView(playerNameCaption);
    playerNameCaption->Show();

    overallNameCaption = new Gui2Caption(windowManager, overall + "role", 4, 0, width_percent, height_percent, overall);
    this->AddView(overallNameCaption);
    overallNameCaption->Show();
  }


  Gui2PlanMapEntry::~Gui2PlanMapEntry() {
  }

}
