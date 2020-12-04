// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "matchoptions.hpp"

#include "main.hpp"

#include "../pagefactory.hpp"

using namespace blunted;

MatchOptionsPage::MatchOptionsPage(Gui2WindowManager *windowManager, const Gui2PageData &pageData) : Gui2Page(windowManager, pageData)
{
  //aggiungi arbitro , stadio?
  Gui2Image *bg = new Gui2Image(windowManager, "matchoptions_image_bg1", 10, 10, 80, 80);
  this->AddView(bg);
  bg->LoadImage("media/menu/backgrounds/black.png");
  bg->Show();

  Gui2Caption *header = new Gui2Caption(windowManager, "matchoptions_caption", 10, 5, 40, 3, "Match options");
  this->AddView(header);
  header->Show();

  Gui2Grid *grid = new Gui2Grid(windowManager, "matchoptions_grid", 15, 15, 70, 70); //29.25, 52, 41.5, 40 - 35, 25, 30, 60

  Gui2Caption *headerDifficulty = new Gui2Caption(windowManager, "matchoptions_captiodifficulty", 0, 0, 15, 3, "Difficulty");
  Gui2Caption *headerDuration = new Gui2Caption(windowManager, "matchoptions_captionduration", 0, 0, 15, 3, "Duration");
  Gui2Caption *headerStadium = new Gui2Caption(windowManager, "matchoptions_captionstadium", 0, 0, 15, 3, "Stadium");
  Gui2Caption *headerRefereeStrict = new Gui2Caption(windowManager, "matchoptions_captionreferee", 0, 0, 15, 3, "Referee Strictness");

  //float difficulty = GetConfiguration()->GetReal("match_difficulty", _default_Difficulty);
  //float matchDuration = GetConfiguration()->GetReal("match_duration", _default_MatchDuration);

  matchOptionsPulldown[0] = new Gui2Pulldown(windowManager, "pulldown_duration", 0, 0, 20, 3);
  matchOptionsPulldown[0]->AddEntry("5 Minutes", "5min");
  matchOptionsPulldown[0]->AddEntry("10 Minutes", "10min");
  matchOptionsPulldown[0]->AddEntry("15 Minutes", "15min");
  matchOptionsPulldown[0]->AddEntry("20 Minutes", "20min");
  matchOptionsPulldown[0]->AddEntry("25 Minutes", "25min");
  matchOptionsPulldown[0]->SetSelected(0);

  matchOptionsPulldown[1] = new Gui2Pulldown(windowManager, "pulldown_difficulty", 0, 0, 20, 3);
  matchOptionsPulldown[1]->AddEntry("Easy", "easy");
  matchOptionsPulldown[1]->AddEntry("Less Easy", "lesseasy");
  matchOptionsPulldown[1]->AddEntry("Medium", "medium");
  matchOptionsPulldown[1]->AddEntry("Hard", "hard");
  matchOptionsPulldown[1]->AddEntry("Ibra Mode", "ibra");
  matchOptionsPulldown[1]->SetSelected(1);

  matchOptionsPulldown[2] = new Gui2Pulldown(windowManager, "pulldown_stadium", 10, 10, 20, 3);
  matchOptionsPulldown[2]->AddEntry("Stadium Minchia", "minchia");
  matchOptionsPulldown[2]->AddEntry("Stadium Potente", "potente");

  Gui2Slider *refereeStrictnessSlider = new Gui2Slider(windowManager, "matchoptions_slider_refereestrictness", 0, 0, 30, 6, "(0 Lenient ..1 Strict)");
  Gui2Button *buttonFormation = new Gui2Button(windowManager, "matchoptions_button_formation", 0, 0, 20, 3, "Formations");

  grid->AddView(headerDifficulty, 0, 0);
  grid->AddView(headerStadium, 1, 0);
  grid->AddView(headerDuration, 2, 0);
  grid->AddView(headerRefereeStrict, 3, 0);

  grid->AddView(matchOptionsPulldown[0], 0, 1);
  grid->AddView(matchOptionsPulldown[1], 1, 1);
  grid->AddView(matchOptionsPulldown[2], 2, 1);
  grid->AddView(refereeStrictnessSlider, 3, 1);

  grid->AddView(buttonFormation, 4, 1);
  grid->UpdateLayout(2.0);

  this->AddView(grid);
  grid->Show();

  buttonFormation->sig_OnClick.connect(boost::bind(&MatchOptionsPage::GoFormationPage, this));
  buttonFormation->SetFocus();
  this->Show();
}

MatchOptionsPage::~MatchOptionsPage()
{
}

void MatchOptionsPage::ProcessWindowingEvent(WindowingEvent *event) {
  //tornare indietro todo
   if (event->IsEscape()) {
    if (windowManager->GetFocus() == buttonFormation) {
      Gui2Page::ProcessWindowingEvent(event);
    }
   }
}

void MatchOptionsPage::GoFormationPage()
{
  float duration = matchOptionsPulldown[0]->GetSelectedIndex() * 0.2f + 0.05f;
  float difficulty = matchOptionsPulldown[1]->GetSelectedIndex() * 0.2f + 0.05f;

  if(duration >= 0 && duration <=1){
    GetConfiguration()->Set("match_duration", duration);
  }else{
    GetConfiguration()->Set("match_duration", 0.05f);
  }

  if(difficulty >= 0 && difficulty <=1){
    GetConfiguration()->Set("match_difficulty", difficulty);
  }else{
    GetConfiguration()->Set("match_difficulty", 0.05f);
  }

  GetConfiguration()->SaveFile(GetConfigFilename());

  this->Exit();

  Properties properties;
  windowManager->GetPageFactory()->CreatePage((int)e_PageID_Formation, properties, 0);

  delete this;
}
