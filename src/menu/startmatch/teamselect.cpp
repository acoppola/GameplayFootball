// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "teamselect.hpp"

#include "../../main.hpp"

#include "utils/database.hpp"

#include "../pagefactory.hpp"

using namespace blunted;

void AddNations(Gui2IconSelector *selector){
  
  //selector->AddEntry("1", "England", "databases/default/images_nations/england.png");
  
  DatabaseResult *result = GetDB()->Query("select id, region_id, name, logo_url from countries");

  for (unsigned int r = 0; r < result->data.size(); r++) {
    int id = atoi(result->data.at(r).at(0).c_str());
    std::string name = result->data.at(r).at(2).c_str();
    std::string logo_url = result->data.at(r).at(3).c_str();

    std::string logoPath = "databases/default/" + logo_url;
    if (!boost::filesystem::exists(logoPath)) logoPath = "media/textures/orange.jpg";
    selector->AddEntry(int_to_str(id), name, logoPath);
  }

  delete result;
}

void AddCompetitions(Gui2IconSelector *selector, const std::string &nation_id) {
  /*
  selector->AddEntry("1", "National teams", "databases/default/images_competitions/nationalteams.png");
  */
  DatabaseResult *result = GetDB()->Query("select id, name, logo_url, level from leagues where country_id = " + nation_id + " order by level");

  for (unsigned int r = 0; r < result->data.size(); r++) {
    int id = atoi(result->data.at(r).at(0).c_str());
    std::string name = result->data.at(r).at(1).c_str();
    std::string logo_url = result->data.at(r).at(2).c_str();

    std::string logoPath = "databases/default/" + logo_url;
    if (!boost::filesystem::exists(logoPath)) logoPath = "media/textures/orange.jpg";
    selector->AddEntry(int_to_str(id), name, logoPath);
  }

  delete result;

  selector->Show();
}

void AddTeams(Gui2IconSelector *selector, const std::string &competition_id) {

  DatabaseResult *result = GetDB()->Query("select id, name, logo_url, kit_url from teams where league_id = " + competition_id + " order by name");

  for (unsigned int r = 0; r < result->data.size(); r++) {
    int id = atoi(result->data.at(r).at(0).c_str());
    std::string name = result->data.at(r).at(1).c_str();
    std::string logo_url = result->data.at(r).at(2).c_str();

    std::string logoPath = "databases/default/" + logo_url;
    if (!boost::filesystem::exists(logoPath)) logoPath = "media/textures/orange.jpg";
    selector->AddEntry(int_to_str(id), name, logoPath);
  }

  delete result;

  selector->Show();
}

TeamSelectPage::TeamSelectPage(Gui2WindowManager *windowManager, const Gui2PageData &pageData) : Gui2Page(windowManager, pageData) {

  Gui2Image *bg1 = new Gui2Image(windowManager, "teamselect_image_bg1", 19, 19, 30, 62);
  this->AddView(bg1);
  bg1->LoadImage("media/menu/backgrounds/black.png");
  bg1->Show();

  bg2 = new Gui2Image(windowManager, "teamselect_image_bg2", 51, 19, 30, 62);
  this->AddView(bg2);
  bg2->LoadImage("media/menu/backgrounds/black.png");
  bg2->Show();

  Gui2Caption *p1 = new Gui2Caption(windowManager, "teamselect_caption_p1", 19, 15, 28, 3, "Player 1");
  p2 = new Gui2Caption(windowManager, "teamselect_caption_p2", 51, 15, 28, 3, "Player 2");
  Gui2Grid *grid1 = new Gui2Grid(windowManager, "teamselect_grid_team1", 19, 19, 30, 61);
  grid2 = new Gui2Grid(windowManager, "teamselect_grid_team2", 51, 19, 30, 61);

  nationSelect1 = new Gui2IconSelector(windowManager, "teamselect_iconselector_nation1", 0, 0, 29, 18, "Nation select");
  nationSelect2 = new Gui2IconSelector(windowManager, "teamselect_iconselector_nation2", 0, 0, 29, 18, "Nation select");
  competitionSelect1 = new Gui2IconSelector(windowManager, "teamselect_iconselector_competition1", 0, 0, 29, 18, "Competition select");
  competitionSelect2 = new Gui2IconSelector(windowManager, "teamselect_iconselector_competition2", 0, 0, 29, 18, "Competition select");
  teamSelect1 = new Gui2IconSelector(windowManager, "teamselect_iconselector_team1", 0, 0, 29, 18, "Team select");
  teamSelect2 = new Gui2IconSelector(windowManager, "teamselect_iconselector_team2", 0, 0, 29, 18, "Team select");
  buttonStart1 = new Gui2Button(windowManager, "teamselect_button_start1", 0, 0, 29, 3, "Ready");
  buttonStart2 = new Gui2Button(windowManager, "teamselect_button_start2", 0, 0, 29, 3, "Ready");

  nationSelect1->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusCompetitionSelect1, this));
  competitionSelect1->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusTeamSelect1, this));
  teamSelect1->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusStart1, this));
  buttonStart1->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusNationSelect2, this));
  nationSelect2->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusCompetitionSelect2, this));
  competitionSelect2->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusTeamSelect2, this));
  teamSelect2->sig_OnClick.connect(boost::bind(&TeamSelectPage::FocusStart2, this));
  buttonStart2->sig_OnClick.connect(boost::bind(&TeamSelectPage::GoOptionsMenu, this));

  competitionSelect1->sig_OnChange.connect(boost::bind(&TeamSelectPage::SetupTeamSelect1, this));
  competitionSelect2->sig_OnChange.connect(boost::bind(&TeamSelectPage::SetupTeamSelect2, this));
  nationSelect1->sig_OnChange.connect(boost::bind(&TeamSelectPage::SetupCompetitionSelect1, this));
  nationSelect2->sig_OnChange.connect(boost::bind(&TeamSelectPage::SetupCompetitionSelect2, this));

  this->AddView(p1);
  p1->Show();
  this->AddView(grid1);
  grid1->AddView(nationSelect1, 0, 0);
  grid1->AddView(competitionSelect1, 1, 0);
  grid1->AddView(teamSelect1, 2, 0);
  grid1->AddView(buttonStart1, 3, 0);
  grid1->UpdateLayout(0.5);
  grid1->Show();

  AddNations(nationSelect1);
  AddCompetitions(competitionSelect1,"1");
  AddTeams(teamSelect1, "1");

  this->AddView(p2);
  this->AddView(grid2);
  grid2->AddView(nationSelect2, 0, 0);
  grid2->AddView(competitionSelect2, 1, 0);
  grid2->AddView(teamSelect2, 2, 0);
  grid2->AddView(buttonStart2, 3, 0);
  grid2->UpdateLayout(0.5);

  AddNations(nationSelect2);
  AddCompetitions(competitionSelect2,"1");
  AddTeams(teamSelect2, "1");

  //competitionSelect1->SetFocus();
  nationSelect1->SetFocus();
  SetActiveController(-1, true);

  p2->Hide();
  grid2->Hide();
  bg2->Hide();

  this->Show();
}

TeamSelectPage::~TeamSelectPage() {
  GetMenuTask()->SetActiveJoystickID(0);
  GetMenuTask()->EnableKeyboard();
}

void TeamSelectPage::FocusCompetitionSelect1() {
  //p2->Show();
  //grid2->Show();
  //bg2->Show();

  //competitionSelect2->SetFocus();
  competitionSelect1->SetFocus();

  //SetActiveController(1, true);
}

void TeamSelectPage::FocusTeamSelect1() {
  teamSelect1->SetFocus();
}

void TeamSelectPage::FocusStart1() {
  buttonStart1->SetFocus();
}

void TeamSelectPage::FocusNationSelect2() {
  p2->Show();
  grid2->Show();
  bg2->Show();

  nationSelect2->SetFocus();

  SetActiveController(1, true);
}

void TeamSelectPage::FocusCompetitionSelect2() {
  //p2->Show();
  //grid2->Show();
  //bg2->Show();

  competitionSelect2->SetFocus();

  //SetActiveController(1, true);
}

void TeamSelectPage::FocusTeamSelect2() {
  teamSelect2->SetFocus();
}

void TeamSelectPage::FocusStart2() {
  buttonStart2->SetFocus();
}

void TeamSelectPage::SetupTeamSelect1() {
  teamSelect1->ClearEntries();
  AddTeams(teamSelect1, competitionSelect1->GetSelectedEntryID());
}

void TeamSelectPage::SetupTeamSelect2() {
  teamSelect2->ClearEntries();
  AddTeams(teamSelect2, competitionSelect2->GetSelectedEntryID());
}

void TeamSelectPage::SetupCompetitionSelect1() {
  competitionSelect1->ClearEntries();
  AddCompetitions(competitionSelect1, nationSelect1->GetSelectedEntryID());
  teamSelect1->ClearEntries();
  AddTeams(teamSelect1, competitionSelect1->GetSelectedEntryID());
}

void TeamSelectPage::SetupCompetitionSelect2() {
  competitionSelect2->ClearEntries();
  AddCompetitions(competitionSelect2, nationSelect2->GetSelectedEntryID());
  teamSelect2->ClearEntries();
  AddTeams(teamSelect2, competitionSelect2->GetSelectedEntryID());
}

void TeamSelectPage::GoOptionsMenu() {
  GetMenuTask()->SetTeamIDs(teamSelect1->GetSelectedEntryID(), teamSelect2->GetSelectedEntryID());
  printf("teams: %i vs %i\n", atoi(teamSelect1->GetSelectedEntryID().c_str()), atoi(teamSelect2->GetSelectedEntryID().c_str()));

  this->Exit();

  Properties properties;
  windowManager->GetPageFactory()->CreatePage((int)e_PageID_MatchOptions, properties, 0);

  delete this;
}

void TeamSelectPage::ProcessWindowingEvent(WindowingEvent *event) {
  //tornare indietro todo
  if (event->IsEscape()) {
    if (windowManager->GetFocus() == nationSelect1) {
      Gui2Page::ProcessWindowingEvent(event);
    } else if (windowManager->GetFocus() == competitionSelect1) {
      windowManager->SetFocus(nationSelect1);
    } else if (windowManager->GetFocus() == teamSelect1) {
      windowManager->SetFocus(competitionSelect1);
    } else if (windowManager->GetFocus() == buttonStart1) {
      windowManager->SetFocus(teamSelect1);
    } else if (windowManager->GetFocus() == nationSelect2) {
      windowManager->SetFocus(buttonStart1);

      p2->Hide();
      grid2->Hide();
      bg2->Hide();

      SetActiveController(-1, true);
    } else if (windowManager->GetFocus() == competitionSelect2) {
      windowManager->SetFocus(nationSelect2);
    } else if (windowManager->GetFocus() == teamSelect2) {
      windowManager->SetFocus(competitionSelect2);
    } else if (windowManager->GetFocus() == buttonStart2) {
      windowManager->SetFocus(teamSelect2);
    }

  }

}
