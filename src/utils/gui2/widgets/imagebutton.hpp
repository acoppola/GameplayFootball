// written by bastiaan konings schuiling 2008 - 2014
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#ifndef _HPP_GUI2_VIEW_IMAGEBUTTON
#define _HPP_GUI2_VIEW_IMAGEBUTTON

#include "SDL2/SDL_ttf.h"

#include "../view.hpp"

#include "scene/objects/image2d.hpp"

#include "caption.hpp"

#include "../windowmanager.hpp"

namespace blunted {

  class Gui2ImageButton : public Gui2View {

    public:
      Gui2ImageButton(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, const std::string &caption, const std::string &filename);
      virtual ~Gui2ImageButton();
      virtual Gui2Caption *GetCaptionWidget() { return captionView; }
      virtual std::string GetCaption() { return captionView->GetCaption(); }
      virtual void SetCaption(const std::string &caption) { captionView->SetCaption(caption); }
      virtual void Process();
      void SetColor(const Vector3 &color);
      virtual void SetToggleable(bool toggleable) { this->toggleable = toggleable; }
      virtual bool IsToggled() { return this->toggled; }
      virtual void SetToggled(bool onOff) { this->toggled = onOff; Redraw(); }
      virtual void SetActive(bool onOff) { this->active = onOff; if (onOff) SetColor(windowManager->GetStyle()->GetColor(e_DecorationType_Bright1)); else SetColor(windowManager->GetStyle()->GetColor(e_DecorationType_Dark2)); Redraw(); }
      virtual void ProcessWindowingEvent(WindowingEvent *event);
      virtual void OnGainFocus();
      virtual void OnLoseFocus();
      virtual void GetImages(std::vector < boost::intrusive_ptr<Image2D> > &target);
      void LoadImage(const std::string &filename);
      virtual void Redraw();
      virtual void SetSize(float new_width_percent, float new_height_percent);
      virtual void SetZoom(float zoomx, float zoomy);
      boost::intrusive_ptr<Image2D> &GetImage2D() { return image; }
      boost::signals2::signal<void(Gui2ImageButton*)> sig_OnClick;
      boost::signals2::signal<void(Gui2ImageButton*)> sig_OnGainFocus;
      boost::signals2::signal<void(Gui2ImageButton*)> sig_OnLoseFocus;

    protected:
      Gui2Caption *captionView;
      boost::intrusive_ptr<Image2D> image;
      boost::intrusive_ptr<Image2D> imageSource;
      int fadeOut_ms;
      int fadeOutTime_ms;
      bool toggleable;
      bool toggled;
      bool active;
      Vector3 color;
  };

}

#endif
