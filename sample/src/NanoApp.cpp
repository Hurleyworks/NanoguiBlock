
#include "NanoApp.h"

NanoApp::NanoApp()
   : gui (nullptr)
{
}

NanoApp::~NanoApp()
{
}

void NanoApp::setup()
{
   fs::path p = getAssetPath("icons");
   gui = std::make_shared<View>();
   auto wnd = getWindow();
   gui->create (wnd, p.parent_path());
}

void NanoApp::update()
{
}

void NanoApp::cleanup()
{
}

void NanoApp::draw()
{
   // performance data
   double t, dt;
   t = getElapsedSeconds();
   dt = t - prevt;
   prevt = t;
   render();
   gui->draw (getElapsedSeconds());
   // Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
   cpuTime = getElapsedSeconds() - t;
   gui->updatePerfGraph ((float)dt, (float)cpuTime);
}

void NanoApp::render()
{
   gl::clear (bgColor);
   gl::pushMatrices();
   gl::setMatrices (camera);
   // gl::ScopedDepth depth (true, true);  // FIXME Generates all sorts of GL errors
   drawGrid();
}

void NanoApp::mouseMove (MouseEvent event)
{
   mousePos = event.getPos();
   gui->mouseMove (event);
}

void NanoApp::mouseDown (MouseEvent event)
{
   usingGui = gui->mouseDown (event);
   // don't update cameraUI if we're using the gui
   if (!usingGui)
      cameraUI.mouseDown (event.getPos());
}

void NanoApp::mouseDrag (MouseEvent event)
{
   mousePos = event.getPos();
   // don't update cameraUI if we're using the gui
   if (!usingGui)
      cameraUI.mouseDrag (event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());
   gui->mouseDrag (event);
}

void NanoApp::mouseUp (MouseEvent event)
{
   gui->mouseUp (event);
   usingGui = false;
}

void NanoApp::keyDown (KeyEvent event)
{
   switch (event.getCode())
   {
      case KeyEvent::KEY_ESCAPE:
         quit();
         break;
      case KeyEvent::KEY_f:
         setFullScreen (!isFullScreen());
         break;
   }

   gui->keyDown(event);
}

void NanoApp::keyUp (KeyEvent event)
{
	gui->keyUp(event);
}

void NanoApp::resize()
{
   camera.setPerspective (60.0f, getWindowAspectRatio(), 0.1f, 1000.0f);
   cameraUI.setCamera (&camera);
   gui->resize (getWindowSize());
}

void NanoApp::drawGrid (float size, float step)
{
   gl::color (Colorf (0.2f, 0.2f, 0.2f));
   for (float i = -size; i <= size; i += step)
   {
      gl::drawLine (vec3 (i, 0.0f, -size), vec3 (i, 0.0f, size));
      gl::drawLine (vec3 (-size, 0.0f, i), vec3 (size, 0.0f, i));
   }
}

