// written by bastiaan konings schuiling 2008 - 2014
// this work is public domain. the code is undocumented, scruffy, untested, and should generally not be used for anything important.
// i do not offer support, so don't ask. to be used for inspiration :)

#include "imagebutton.hpp"

#include "SDL2/SDL2_rotozoom.h"

namespace blunted {

  Gui2ImageButton::Gui2ImageButton(Gui2WindowManager *windowManager, const std::string &name, float x_percent, float y_percent, float width_percent, float height_percent, const std::string &caption, const std::string &filename) : Gui2View(windowManager, name, x_percent, y_percent, width_percent, height_percent) {

    isSelectable = true;

    color = windowManager->GetStyle()->GetColor(e_DecorationType_Bright1);

    int x, y, w, h;
    windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);
    image = windowManager->CreateImage2D(name, w, h, true);

    captionView = new Gui2Caption(windowManager, name + "caption", 1.2, height_percent-3 /*0.3*/, 20, 3 - 0.6, caption);
    this->AddView(captionView);
    captionView->Show();

    fadeOutTime_ms = 200;
    fadeOut_ms = fadeOutTime_ms;

    toggleable = false;
    toggled = false;
    active = true;
  
    SDL_Surface *imageSurfTmp = IMG_Load(filename.c_str());
    imageSource = windowManager->CreateImage2D(name + "source", imageSurfTmp->w, imageSurfTmp->h, false);
    boost::intrusive_ptr < Resource<Surface> > surfaceRes = imageSource->GetImage();
    surfaceRes->resourceMutex.lock();
    surfaceRes->GetResource()->SetData(imageSurfTmp);
    surfaceRes->resourceMutex.unlock();

    Redraw();
  }

  void Gui2ImageButton::LoadImage(const std::string &filename) {
    SDL_Surface *imageSurfTmp = IMG_Load(filename.c_str());
    imageSource = windowManager->CreateImage2D(name + "source", imageSurfTmp->w, imageSurfTmp->h, false);
    boost::intrusive_ptr < Resource<Surface> > surfaceRes = imageSource->GetImage();
    surfaceRes->resourceMutex.lock();
    surfaceRes->GetResource()->SetData(imageSurfTmp);
    surfaceRes->resourceMutex.unlock();
    Redraw();
  }

  Gui2ImageButton::~Gui2ImageButton() {
  }

  void Gui2ImageButton::GetImages(std::vector < boost::intrusive_ptr<Image2D> > &target) {
    target.push_back(image);
    Gui2View::GetImages(target);
  }

  void Gui2ImageButton::Process() {
    if (fadeOut_ms <= fadeOutTime_ms) {
      fadeOut_ms += windowManager->GetTimeStep_ms();
      if (!IsFocussed() && fadeOut_ms <= fadeOutTime_ms) { // cool fadeout effect!
        Redraw();
      }
    }

    Gui2View::Process();
  }

  void Gui2ImageButton::SetColor(const Vector3 &color) {
    this->color = color;
    captionView->SetColor(color);
    Redraw();
  }

  void Gui2ImageButton::Redraw() {
    int x, y, w, h;
    windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);
    float x_ratio = w / width_percent; // 1% width
    float y_ratio = h / height_percent; // 1% width
    int x_margin = int(round(x_ratio * 0.5));
    int y_margin = int(round(y_ratio * 0.5));

    int alpha = 0;
    Vector3 color1;
    if (imageSource != boost::intrusive_ptr<Image2D>()) {
      boost::intrusive_ptr < Resource<Surface> > surfaceRes = imageSource->GetImage();
      surfaceRes->resourceMutex.lock();
      SDL_Surface *imageSurfTmp = surfaceRes->GetResource()->GetData();
      double zoomx;
      zoomx = (double)w / imageSurfTmp->w;
      double zoomy;
      zoomy = (double)h / imageSurfTmp->h;
      SDL_Surface *imageSurf = zoomSurface(imageSurfTmp, zoomx, zoomy, 1);
      surfaceRes->resourceMutex.unlock();
      surfaceRes = image->GetImage();
      surfaceRes->resourceMutex.lock();
      surfaceRes->GetResource()->SetData(imageSurf);
      surfaceRes->resourceMutex.unlock();
    }
    Vector3 color2 = Vector3(192,192,192);
    color1 = windowManager->GetStyle()->GetColor(e_DecorationType_Bright2);
    alpha = 255;
    if (IsFocussed()) {
      image->DrawRectangle(0, 3*h/4, w, h/4, color2, 200);
    }else{
      image->DrawRectangle(0, 3*h/4, w, h/4, color1, 200);
    }
    if (!active) {
      image->DrawRectangle(0, 3*h/4, w, h/4, Vector3(25, 25, 25), alpha);
    }
    image->OnChange();
  }

  void Gui2ImageButton::ProcessWindowingEvent(WindowingEvent *event) {
    if (event->IsActivate() && active) {
      if (toggleable) {
        if (toggled) { toggled = false; } else { toggled = true; }
        Redraw();
      }
      sig_OnClick(this);
    } else {
      event->Ignore();
    }
  }

  void Gui2ImageButton::OnGainFocus() {
    Redraw();
    sig_OnGainFocus(this);
  }

  void Gui2ImageButton::OnLoseFocus() {
    fadeOut_ms = 0;
    sig_OnLoseFocus(this);
  }

  void Gui2ImageButton::SetSize(float new_width_percent, float new_height_percent) {
    Gui2View::SetSize(new_width_percent, new_height_percent);

    int x, y, w, h;
    windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);
    //printf("resized to %i %i\n", w, h);

    image->Resize(w, h);
    Redraw();
  }

  void Gui2ImageButton::SetZoom(float zoomx, float zoomy) {

    // paste source image onto screen image
    if (imageSource != boost::intrusive_ptr<Image2D>()) {

      // get image
      boost::intrusive_ptr < Resource<Surface> > surfaceRes = imageSource->GetImage();
      surfaceRes->resourceMutex.lock();

      SDL_Surface *imageSurfTmp = surfaceRes->GetResource()->GetData();

      int x, y, w, h;
      windowManager->GetCoordinates(x_percent, y_percent, width_percent, height_percent, x, y, w, h);

      double zoomx1;
      zoomx1 = (double)w / imageSurfTmp->w * zoomx;
      double zoomy1;
      zoomy1 = (double)h / imageSurfTmp->h * zoomy;
      SDL_Surface *imageSurf = zoomSurface(imageSurfTmp, zoomx1, zoomy1, 1);
      //printf("actually resized to %i %i\n", imageSurf->w, imageSurf->h);

      surfaceRes->resourceMutex.unlock();

      image->DrawRectangle(0, 0, w, h, Vector3(0, 0, 0), 0);

      surfaceRes = image->GetImage();
      surfaceRes->resourceMutex.lock();

      SDL_Surface *surface = surfaceRes->GetResource()->GetData();
      SDL_Rect rect;
      rect.x = w * 0.5 - imageSurf->w * 0.5;
      rect.y = h * 0.5 - imageSurf->h * 0.5;
      SDL_BlitSurface(imageSurf, NULL, surface, &rect);

      surfaceRes->resourceMutex.unlock();

      SDL_FreeSurface(imageSurf);

      image->OnChange();
    }
  }

}
