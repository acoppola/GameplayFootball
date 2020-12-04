// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "playerdetail.hpp"

#include "utils/gui2/windowmanager.hpp"

#include "utils/gui2/widgets/image.hpp"

#include "../../gamedefines.hpp"

#include "../../data/teamdata.hpp"

namespace blunted {

  Gui2PlayerDetail::Gui2PlayerDetail(Gui2WindowManager *windowManager, const std::string &name, int posrow, float x_percent, float y_percent, float width_percent, float height_percent/*, TeamData *teamData*/) : Gui2View(windowManager, name, x_percent, y_percent, width_percent, height_percent) {

    int x, y, w, h;
    windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);
    image = windowManager->CreateImage2D(name, w, h, true);

    captionView = new Gui2Caption(windowManager, name + "caption", 1.2, 0.3, 10, 3, "caption");
    //this->AddView(captionView);
    //captionView->Show();
    playerData = new PlayerData(10);
    Redraw();
  }

  Gui2PlayerDetail::~Gui2PlayerDetail() {
      TTF_CloseFont(font);
  }

  void Gui2PlayerDetail::GetImages(std::vector < boost::intrusive_ptr<Image2D> > &target) {
    target.push_back(image);
    Gui2View::GetImages(target);
  }

  void Gui2PlayerDetail::Process() {
    Gui2View::Process();
  }

  void Gui2PlayerDetail::Redraw() {
    int x, y, w, h;
    windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);
    float x_ratio = w / width_percent; // 1% width
    float y_ratio = h / height_percent; // 1% width
    int x_margin = int(round(x_ratio * 0.5));
    int y_margin = int(round(y_ratio * 0.5));
    Vector3 color1 = windowManager->GetStyle()->GetColor(e_DecorationType_Bright2);
    Vector3 color2 = windowManager->GetStyle()->GetColor(e_DecorationType_Bright1);
    font = TTF_OpenFont("media/blunted/HUD/terminus.fon", 12);
    image->DrawRectangle(0, 0, w, h, color2, 200);
    image->DrawRectangle(x_margin, y_margin, w - x_margin * 2, h - y_margin * 2, color1, 0);
    if(hasPlayerData){
        image->DrawSimpleText(playerData->GetLastName(), 10, 5, font, Vector3(200, 200, 200), 255);
        image->DrawSimpleText(playerData->GetFirstName(), 10, 15, font, Vector3(200, 200, 200), 255);
    }else{
        image->DrawSimpleText("cap", 0, 0, font,Vector3(200, 200, 200), 255);
        //image->DrawSimpleText(playerData->GetLastName(), 0, 0, font,Vector3(200, 200, 200), 255);
    }

    image->OnChange();
  }

  void Gui2PlayerDetail::SetPlayerData(PlayerData *pd) {
      playerData = pd;
      hasPlayerData = true;
      Redraw();
  }

  Gui2PlayerGraph::Gui2PlayerGraph(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, const std::string &roleName, const std::string &playerName, const std::string &overall) : Gui2View(windowManager, name, x_percent, y_percent, width_percent, height_percent) {
  }


  Gui2PlayerGraph::~Gui2PlayerGraph() {
  }

}
