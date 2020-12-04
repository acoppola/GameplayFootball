// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#ifndef _HPP_GUI2_VIEW_PLAYERDET
#define _HPP_GUI2_VIEW_PLAYERDET

#include "utils/gui2/view.hpp"
#include "utils/gui2/widgets/caption.hpp"

#include "scene/objects/image2d.hpp"
#include "../../data/playerdata.hpp"

class TeamData;
class Match;

namespace blunted {

  class Gui2PlayerDetail : public Gui2View {

    public:
      Gui2PlayerDetail(Gui2WindowManager *windowManager, const std::string &name, int posrow, float x_percent, float y_percent, float width_percent, float height_percent/*, TeamData *teamData*/);
      virtual ~Gui2PlayerDetail();
      virtual void GetImages(std::vector < boost::intrusive_ptr<Image2D> > &target);
      virtual void Redraw();
      virtual void Process();
      virtual void SetPlayerData(PlayerData *pd);

    protected:
      boost::intrusive_ptr<Image2D> image;
      Gui2Caption *captionView;
      SDL_Surface *bg;
      TeamData *teamData;
      PlayerData *playerData;
      std::string surname;
      bool hasPlayerData;
      TTF_Font *font;

  };

  class Gui2PlayerGraph : public Gui2View {

    public:
      Gui2PlayerGraph(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, const std::string &roleName, const std::string &playerName, const std::string &overall);
      virtual ~Gui2PlayerGraph();

    protected:
      SDL_Surface *zoomedImage;

  };

}

#endif
