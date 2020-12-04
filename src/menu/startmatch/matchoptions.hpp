// written by bastiaan konings schuiling 2008 - 2015
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#ifndef _HPP_MENU_MATCHOPTIONS
#define _HPP_MENU_MATCHOPTIONS

#include "utils/gui2/windowmanager.hpp"

#include "utils/gui2/widgets/menu.hpp"
#include "utils/gui2/widgets/root.hpp"
#include "utils/gui2/widgets/grid.hpp"
#include "utils/gui2/widgets/button.hpp"
#include "utils/gui2/widgets/image.hpp"
#include "utils/gui2/widgets/slider.hpp"
#include "utils/gui2/widgets/iconselector.hpp"
#include "utils/gui2/widgets/pulldown.hpp"

using namespace blunted;

class MatchOptionsPage : public Gui2Page {

  public:
    MatchOptionsPage(Gui2WindowManager *windowManager, const Gui2PageData &pageData);
    virtual ~MatchOptionsPage();
    virtual void ProcessWindowingEvent(WindowingEvent *event);

    void GoFormationPage();

    Gui2Button *buttonFormation;

  protected:
    Gui2Slider *refereeStrictnessSlider;
    Gui2Image *bg;
    //Gui2Pulldown *stadiumPulldown;
    //Gui2Pulldown *durationPulldown;
    //Gui2Pulldown *difficultyPulldown;
    Gui2Pulldown *matchOptionsPulldown[3];

};

#endif
