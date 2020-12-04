// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "formation.hpp"

#include "main.hpp"

#include "utils/database.hpp"

#include "../pagefactory.hpp"
#include "base/log.hpp"

using namespace blunted;

FormationPage::FormationPage(Gui2WindowManager *windowManager, const Gui2PageData &pageData) : Gui2Page(windowManager, pageData) {

  //Gui2Image *bg = new Gui2Image(windowManager, "formation_image_bg1", 5, 5, 90, 90);
  //this->AddView(bg);
  //bg->LoadImage("media/menu/backgrounds/black.png");
  //bg->Show();

  MatchData *matchData = new MatchData(GetMenuTask()->GetTeamID(0), GetMenuTask()->GetTeamID(1));
  GetMenuTask()->SetMatchData(matchData);
  teamData1 = matchData->GetTeamData(0);
  teamData2 = matchData->GetTeamData(1);

  // this->sig_OnClose.connect(boost::bind(&FormationPage::OnClose, this));
  Gui2Caption *header_one = new Gui2Caption(windowManager, "gameplan_header_one", 10, 5, 35, 3, "Team 1 game plan");
  Gui2Caption *header_two = new Gui2Caption(windowManager, "gameplan_header_two", 60, 5, 35, 3, "Team 2 game plan");
  this->AddView(header_one);
  this->AddView(header_two);
  header_one->Show();
  header_two->Show();

  grid_one = new Gui2Grid(windowManager, "gameplan_grid_one", 5, 9, 0, 0);
  gridNav_one = new Gui2Grid(windowManager, "gameplan_grid_navigation_one", 10, 0, 0, 0);
  grid_two = new Gui2Grid(windowManager, "gameplan_grid_two", 55, 9, 0, 0);
  gridNav_two = new Gui2Grid(windowManager, "gameplan_grid_navigation_two", 10, 0, 0, 0);

  map_one = new Gui2PlanMap(windowManager, "gameplan_planmap_one", 0, 0, 40, 25, teamData1);
  map_two = new Gui2PlanMap(windowManager, "gameplan_planmap_two", 0, 0, 40, 25, teamData2);

  buttonLineup_one = new Gui2Button(windowManager, "gameplan_button_lineup_one", 0, 0, 34, 3, "Line-up");
  buttonTactics_one = new Gui2Button(windowManager, "gameplan_button_tactics_one", 0, 0, 34, 3, "Tactics");
  buttonLineup_two = new Gui2Button(windowManager, "gameplan_button_lineup_two", 0, 0, 34, 3, "Line-up");
  buttonTactics_two = new Gui2Button(windowManager, "gameplan_button_tactics_two", 0, 0, 34, 3, "Tactics");
  buttonReady_one = new Gui2Button(windowManager, "gameplan_button_ready_one", 0, 0, 34, 3, "Ready");
  buttonReady_two = new Gui2Button(windowManager, "gameplan_button_ready_two", 0, 0, 34, 3, "Ready");

  buttonLineup_one->sig_OnClick.connect(boost::bind(&FormationPage::GoLineupMenu_one, this));
  buttonTactics_one->sig_OnClick.connect(boost::bind(&FormationPage::GoTacticsMenu_one, this));
  buttonLineup_two->sig_OnClick.connect(boost::bind(&FormationPage::GoLineupMenu_two, this));
  buttonTactics_two->sig_OnClick.connect(boost::bind(&FormationPage::GoTacticsMenu_two, this));
  buttonReady_one->sig_OnClick.connect(boost::bind(&FormationPage::GoReady_one, this));
  buttonReady_two->sig_OnClick.connect(boost::bind(&FormationPage::GoReady_two, this));

  Gui2Button *buttonStart = new Gui2Button(windowManager, "formation_button_start", 30, 90, 30, 3, "Start match");
  this->AddView(buttonStart);

  this->AddView(grid_one);
  this->AddView(grid_two);

  gridNav_one->AddView(buttonLineup_one, 0, 0);
  gridNav_one->AddView(buttonTactics_one, 1, 0);
  gridNav_one->AddView(buttonReady_one, 2, 0);
  gridNav_two->AddView(buttonLineup_two, 0, 0);
  gridNav_two->AddView(buttonTactics_two, 1, 0);
  gridNav_two->AddView(buttonReady_two, 2, 0);
  gridNav_one->UpdateLayout(0.5);
  gridNav_two->UpdateLayout(0.5);
  grid_one->AddView(map_one, 0, 0);
  grid_two->AddView(map_two, 0, 0);
  grid_one->AddView(gridNav_one, 1, 0);
  grid_two->AddView(gridNav_two, 1, 0);

  // buttonStart->Show();
  buttonStart->sig_OnClick.connect(boost::bind(&FormationPage::GoLoadingMatchPage, this));
  buttonReady_two->sig_OnClick.connect(boost::bind(&FormationPage::GoLoadingMatchPage, this));
  //buttonStart->SetFocus();
  buttonLineup_one->SetFocus();

  grid_one->UpdateLayout(0.0);
  grid_two->UpdateLayout(0.0);
  grid_one->Show();
  grid_two->Show();

  this->Show();
  if (UpdateNonImportableDB()) {
    namedb = new Database();
    bool dbSuccess = namedb->Load("databases/names.sqlite");
    if (!dbSuccess) Log(e_FatalError, "MainMenuPage", "GoImportDB", "Could not open database");
  } else {
    namedb = 0;
  }
}

FormationPage::~FormationPage() {
}

void FormationPage::GoLoadingMatchPage() {

  this->Exit();

  Properties properties;
  windowManager->GetPageFactory()->CreatePage((int)e_PageID_LoadingMatch, properties, 0);

  delete this;
}

Vector3 FormationPage::GetButtonColor(int id) {
  Vector3 color = windowManager->GetStyle()->GetColor(e_DecorationType_Bright1);
  if (id > 10) color = Vector3(240, 140, 60);
  if (id > 17) color = Vector3(80, 240, 140);
  if (id > 21) color = Vector3(80, 140, 255);
  return color;
}

void FormationPage::SaveLineup(int part) {
  if(part == 0){
    teamData1->SaveLineup();
  }else{
    teamData2->SaveLineup();
  }
}

void FormationPage::LineupMenuOnFocus(Gui2Button *button, int part) {
  GamePlanSubMenu *lineupMenu = 0;
  if(part == 0){
    lineupMenu = lineupMenu_one;
    int rowButton = lineupMenu->GetGrid()->GetRow(button);
    int id = atoi(button->GetName().substr(button->GetName().rfind("id") + 2, std::string::npos).c_str());
    lineupMenu->SetPlayerData(teamData1->GetPlayerData(rowButton));
  }else{
    lineupMenu = lineupMenu_two;
    int rowButton = lineupMenu->GetGrid()->GetRow(button);
    int id = atoi(button->GetName().substr(button->GetName().rfind("id") + 2, std::string::npos).c_str());
    lineupMenu->SetPlayerData(teamData2->GetPlayerData(rowButton));
  }
  
}

void FormationPage::LineupMenuOnClick(Gui2Button *button, int part) {
  GamePlanSubMenu *lineupMenu = 0;
  if(part == 0){
    lineupMenu = lineupMenu_one;
  }else{
    lineupMenu = lineupMenu_two;
  }
  Gui2Button *selected = lineupMenu->GetToggledButton(button);
  if (selected) {
    // switch playerss
    selected->SetToggled(false);
    button->SetToggled(false);
    int rowSelected = lineupMenu->GetGrid()->GetRow(selected);
    int rowButton = lineupMenu->GetGrid()->GetRow(button);
    assert(rowSelected != -1 && rowButton != -1);
    lineupMenu->GetGrid()->RemoveView(rowSelected, 0);
    lineupMenu->GetGrid()->RemoveView(rowButton, 0);
    lineupMenu->GetGrid()->AddView(button, rowSelected, 0);
    button->Show(); 
    button->SetColor(GetButtonColor(rowSelected));
    lineupMenu->GetGrid()->AddView(selected, rowButton, 0);
    selected->Show();
    selected->SetColor(GetButtonColor(rowButton));
    lineupMenu->GetGrid()->UpdateLayout(0.5);
    selected->SetFocus();
    int id1 = atoi(selected->GetName().substr(selected->GetName().rfind("id") + 2, std::string::npos).c_str());
    int id2 = atoi(  button->GetName().substr(  button->GetName().rfind("id") + 2, std::string::npos).c_str());
    if(part == 0){
      teamData1->SwitchPlayers(id1, id2);
      map_one->Redraw();
    }else{
      teamData2->SwitchPlayers(id1, id2);
      map_two->Redraw();
    }
  }
}

void FormationPage::GoLineupMenu_one() {
  Deactivate(0);
  lineupMenu_one = new GamePlanSubMenu(windowManager, buttonLineup_one, grid_one, "lineup_submenu_one");
  lineupMenu_one->sig_OnClose.connect(boost::bind(&FormationPage::SaveLineup, this, 0));
  lineupMenu_one->sig_OnClose.connect(boost::bind(&FormationPage::Reactivate, this, 0));

  const std::vector<PlayerData*> &playerData1 = teamData1->GetPlayerData();
  for (unsigned int i = 0; i < playerData1.size(); i++) {
    Vector3 color = GetButtonColor(i);
    std::string playerFullName;
    playerFullName.append(GetRoleName(playerData1.at(i)->GetRoles()[0]));
    playerFullName.append(" - ");
    playerFullName.append(playerData1.at(i)->GetLastName());
    playerFullName.append(" ");
    Gui2Button *button = lineupMenu_one->AddButton("playerbuttonone_id" + int_to_str(playerData1.at(i)->GetDatabaseID()), playerFullName, i, 0, color);
    button->sig_OnClick.connect(boost::bind(&FormationPage::LineupMenuOnClick, this, _1, 0));
    button->sig_OnGainFocus.connect(boost::bind(&FormationPage::LineupMenuOnFocus, this, _1, 0));
    button->SetToggleable(true);
    if (i == 0) button->SetFocus();
  }
  lineupMenu_one->Show();
}

void FormationPage::GoLineupMenu_two() {
  Deactivate(1);
  lineupMenu_two = new GamePlanSubMenu(windowManager, buttonLineup_two, grid_two, "lineup_submenu_two");
  lineupMenu_two->sig_OnClose.connect(boost::bind(&FormationPage::SaveLineup, this, 1));
  lineupMenu_two->sig_OnClose.connect(boost::bind(&FormationPage::Reactivate, this, 1));

  const std::vector<PlayerData*> &playerData2 = teamData2->GetPlayerData();
  for (unsigned int i = 0; i < playerData2.size(); i++) {
    Vector3 color = GetButtonColor(i);
    std::string playerFullName;
    playerFullName.append(GetRoleName(playerData2.at(i)->GetRoles()[0]));
    playerFullName.append(" - ");
    playerFullName.append(playerData2.at(i)->GetLastName());
    playerFullName.append(" ");
    Gui2Button *button = lineupMenu_two->AddButton("playerbuttontwo_id" + int_to_str(playerData2.at(i)->GetDatabaseID()), playerFullName, i, 0, color);
    button->sig_OnClick.connect(boost::bind(&FormationPage::LineupMenuOnClick, this, _1, 1));
    button->sig_OnGainFocus.connect(boost::bind(&FormationPage::LineupMenuOnFocus, this, _1, 1));
    button->SetToggleable(true);
    if (i == 0) button->SetFocus();
  }

  lineupMenu_two->Show();
}

void FormationPage::GoTacticsMenu_one() {
  Deactivate(0);

  tacticsSliders_one.clear(); // could still be here from previous tactics subpage visit. should clear this on leaving page but we have no mechanism to do so (<- todo, could use onclose probably)

  tacticsMenu_one = new GamePlanSubMenu(windowManager, buttonTactics_one, grid_one, "tactics_submenu_one");
  tacticsMenu_one->sig_OnClose.connect(boost::bind(&FormationPage::SaveTactics_one, this));
  tacticsMenu_one->sig_OnClose.connect(boost::bind(&FormationPage::Reactivate, this, 0));

  const Properties &userProps = teamData1->GetTactics().userProperties;
  const map_Properties *userPropMap = userProps.GetProperties();
  const Properties &factoryProps = teamData1->GetTactics().factoryProperties;
  const map_Properties *factoryPropMap = factoryProps.GetProperties();

  map_Properties::const_iterator iter = userPropMap->begin();
  int i = 0;
  while (iter != userPropMap->end()) {
    const std::string &tacticName = (*iter).first;
    if (Verbose()) printf("adding %s\n", tacticName.c_str());
    TacticsSliderFormation slider;
    slider.id = i;
    slider.tacticName = tacticName;
    slider.widget = tacticsMenu_one->AddSlider("tacticssliderone_" + slider.tacticName, teamData1->GetTactics().humanReadableNames.Get(slider.tacticName.c_str(), slider.tacticName), i, 0);
    slider.widget->AddHelperValue(Vector3(80, 80, 250), "factory default for this team", factoryProps.GetReal(slider.tacticName.c_str()));
    slider.widget->SetValue(userProps.GetReal(slider.tacticName.c_str()));
    slider.widget->sig_OnChange.connect(boost::bind(&FormationPage::TacticsMenuOnChange, this, _1, slider.id, 0));
    if (i == 0) slider.widget->SetFocus();
    tacticsSliders_one.push_back(slider);
    i++;
    iter++;
  }

  tacticsMenu_one->Show();
}

void FormationPage::GoTacticsMenu_two() {
}

void FormationPage::GoReady_one() {
  buttonLineup_two->SetFocus();
}

void FormationPage::GoReady_two() {
  
}

void FormationPage::Deactivate(int part) {
  if(part == 0){
    grid_one->RemoveView(1, 0);
  }else{
    grid_two->RemoveView(1, 0);
  }
}

void FormationPage::Reactivate(int part) {
  if(part == 0){
    grid_one->AddView(gridNav_one, 1, 0);
    grid_one->UpdateLayout(0.0);
    gridNav_one->Show();
  }else{
    grid_two->AddView(gridNav_two, 1, 0);
    grid_two->UpdateLayout(0.0);
    gridNav_two->Show();
  }
}

void FormationPage::TacticsMenuOnChange(Gui2Slider *slider, int id, int part) {
  //printf("slider %i (%s) altered\n", id, slider->GetName().c_str());
  if(part == 0) {
    Properties &userProps = teamData1->GetTacticsWritable().userProperties;
    userProps.Set(tacticsSliders_one.at(id).tacticName.c_str(), tacticsSliders_one.at(id).widget->GetValue());
  }
  if(part == 1) {
    Properties &userProps = teamData2->GetTacticsWritable().userProperties;
    userProps.Set(tacticsSliders_two.at(id).tacticName.c_str(), tacticsSliders_two.at(id).widget->GetValue());
  }
}

void FormationPage::SaveTactics_one() {

  if (UpdateNonImportableDB()) {
    // saves to temp names db, which is used when importing the actual db.

    std::string tactics_xml;

    for (unsigned int i = 0; i < tacticsSliders_one.size(); i++) {
      tactics_xml += "<" + tacticsSliders_one.at(i).tacticName + ">" + real_to_str(tacticsSliders_one.at(i).widget->GetValue()) + "</" + tacticsSliders_one.at(i).tacticName + ">\n";
    }
    printf("tactics:\n%s\n", tactics_xml.c_str());

    // find club
    DatabaseResult *result = namedb->Query("select id from clubnames where faketargetname = \"" + teamData1->GetName() + "\" limit 1;");
    int teamDatabaseID = -1;
    if (result->data.size() > 0) {
      teamDatabaseID = atoi(result->data.at(0).at(0).c_str());
      delete result;
      result = namedb->Query("update clubnames set tactics_xml = \"" + tactics_xml + "\" where id = " + int_to_str(teamDatabaseID) + ";");
      delete result;
    } else { // team does not yet exist in namedb
      if (Verbose()) printf("WARNING: team does not exist in namedb: %s\n", teamData1->GetName().c_str());
      delete result;
    }
  }

  teamData1->SaveTactics();
}
void FormationPage::ProcessWindowingEvent(WindowingEvent *event) {
  //tornare indietro todo
   if (event->IsEscape()) {
    if (windowManager->GetFocus() == buttonLineup_one) {
      Gui2Page::ProcessWindowingEvent(event);
    }else if (windowManager->GetFocus() == buttonStart) {
       windowManager->SetFocus(buttonLineup_two);
    } else if (windowManager->GetFocus() == buttonLineup_two) {
      windowManager->SetFocus(buttonReady_one);
    }
   }
}
