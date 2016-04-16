#include "Standard.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"

#include "gui/View.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class NanoApp : public App
{

   public:
      NanoApp();
      ~NanoApp();

      void setup() override;
      void resize() override;
      void update() override;
      void draw() override;

      void mouseMove (MouseEvent event) override;
      void mouseDown (MouseEvent event) override;
      void mouseDrag (MouseEvent event) override;
      void mouseUp (MouseEvent event) override;

      void keyDown (KeyEvent event) override;
      void keyUp (KeyEvent event) override;

   private:
      ViewRef gui;
      double prevt = 0;
      double cpuTime = 0;
      bool usingGui = false;
      CameraPersp camera;
      CameraUi cameraUI;
      ivec2 mousePos;
      Color bgColor = Color (0.1f, 0.11f, 0.12f);

      void render();
      void cleanup() override;

      void drawGrid (float size = 100.0f, float step = 10.0f);
};

CINDER_APP (NanoApp, RendererGl (RendererGl::Options().stencil().msaa (16)),
            [&] (App::Settings * settings)
{
   settings->setWindowSize (appWidth, appHeight);
   settings->setFrameRate (60.0f);
   settings->setTitle (getTitle());
   settings->setHighDensityDisplayEnabled();

})
