/*
   src/screen.cpp -- Top-level widget and interface between NanoGUI and GLFW

   A significant redesign of this code was contributed by Christian Schueller.

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

// Allow enforcing the GL3 implementation of NanoVG 
#define NANOVG_GL3_IMPLEMENTATION
#include "../nanovg/nanovg_gl.h"

NAMESPACE_BEGIN (nanogui)

// ctor
Screen::Screen()
   : Widget (nullptr)
{
#ifdef NDEBUG
   mNVGContext = nvgCreateGL3 (NVG_STENCIL_STROKES | NVG_ANTIALIAS);
#else
   mNVGContext = nvgCreateGL3 (NVG_STENCIL_STROKES | NVG_ANTIALIAS | NVG_DEBUG);
#endif
   if (mNVGContext == nullptr)
      throw std::runtime_error ("Could not initialize NanoVG!");
   start = std::chrono::system_clock::now();
}

// dtor
Screen::~Screen ()
{
   if (mNVGContext)
      nvgDeleteGL3 (mNVGContext);
}

void Screen::drawWidgets()
{
   if (!mVisible)
      return;
   float aspect = (float)mSize[0] / (float)mSize[1];
   nvgBeginFrame (mNVGContext, mSize[0], mSize[1], aspect);
   draw (mNVGContext);
   // work around for Cinder not rendering after nanovg
   ci::gl::ScopedGlslProg scopedProg (nullptr);
   ci::gl::ScopedVao scopedVao (nullptr);
   ci::gl::ScopedTextureBind text2 (GL_TEXTURE_2D, 0);
   ci::gl::ScopedTextureBind text3(GL_TEXTURE_3D, 0);
   ci::gl::ScopedDepth depth(false, false);  // FIXME causes GL errors

   nvgEndFrame (mNVGContext);
}

bool Screen::cursorPosCallbackEvent (double x, double y)
{
   auto end = std::chrono::system_clock::now();
   ivec2 p ((int)x, (int)y);
   bool ret = false;
   mLastInteraction = end - start;
   try
   {
      p -= ivec2 (1, 2);
      if (!mDragActive)
      {
         Widget * const widget = findWidget (p);
         // No cursor support in Cinder?????
         //if (widget != nullptr && widget->cursor() != mCursor)
         //{
         //	mCursor = widget->cursor();
         //	//glfwSetCursor(mGLFWWindow, mCursors[(int)mCursor]);
         //}
      }
      else
      {
         ret = mDragWidget->mouseDragEvent (p - mDragWidget->parent()->absolutePosition(),
                                            p - mMousePos,
                                            mMouseState, mModifiers);
      }
      if (!ret)
         ret = mouseMotionEvent (p, p - mMousePos, mMouseState, mModifiers);
      mMousePos = p;
      return ret;
   }
   catch (const std::exception & e)
   {
      std::cout << "Caught exception in event handler: " << e.what() << std::endl;
   }
   return false;
}

bool Screen::mouseButtonCallbackEvent (int button, int action, int modifiers)
{
   auto end = std::chrono::system_clock::now();
   mModifiers = modifiers;
   mLastInteraction = end - start;
   try
   {
      if (mFocusPath.size() > 1)
      {
         const Window * window =
            dynamic_cast<Window *> (mFocusPath[mFocusPath.size() - 2]);
         if (window && window->modal())
         {
            if (!window->contains (mMousePos))
               return false;
         }
      }
      if (action == PRESS)
         mMouseState |= 1 << button;
      else
         mMouseState &= ~ (1 << button);
      if (action == PRESS && button == MOUSE_BUTTON_LEFT)
      {
         mDragWidget = findWidget (mMousePos);
         if (mDragWidget == this)
            mDragWidget = nullptr;
         mDragActive = mDragWidget != nullptr;
         if (!mDragActive)
            updateFocus (nullptr);
      }
      else
      {
         mDragActive = false;
         mDragWidget = nullptr;
      }
      return mouseButtonEvent (mMousePos, button, action == PRESS, mModifiers);
   }
   catch (const std::exception & e)
   {
      std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
      //abort();
   }
   return false;
}

bool Screen::keyCallbackEvent (int key, int scancode, int action, int mods)
{
   auto end = std::chrono::system_clock::now();
   mLastInteraction = end - start;
   try
   {
      return keyboardEvent (key, scancode, action, mods);
   }
   catch (const std::exception & e)
   {
      std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
      //abort();
   }

   return false;
}

bool Screen::keyboardEvent (int key, int scancode, int action, int modifiers)
{
   if (mFocusPath.size() > 0)
   {
      for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
         if ((*it)->focused() && (*it)->keyboardEvent (key, scancode, action, modifiers))
            return true;
   }
   return false;
}

bool Screen::keyboardCharacterEvent (unsigned int codepoint)
{
   if (mFocusPath.size() > 0)
   {
      for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
         if ((*it)->focused() && (*it)->keyboardCharacterEvent (codepoint))
            return true;
   }
   return false;
}

bool Screen::charCallbackEvent (unsigned int codepoint)
{
   auto end = std::chrono::system_clock::now();
   mLastInteraction = end - start;
   try
   {
      return keyboardCharacterEvent (codepoint);
   }
   catch (const std::exception & e)
   {
      std::cerr << "Caught exception in event handler: " << e.what()
                << std::endl;
      abort();
   }
}

void Screen::updateFocus (Widget * widget)
{
   for (auto w : mFocusPath)
   {
      if (!w->focused())
         continue;
      w->focusEvent (false);
   }
   mFocusPath.clear();
   Widget * window = nullptr;
   while (widget)
   {
      mFocusPath.push_back (widget);
      if (dynamic_cast<Window *> (widget))
         window = widget;
      widget = widget->parent();
   }
   for (auto it = mFocusPath.rbegin(); it != mFocusPath.rend(); ++it)
      (*it)->focusEvent (true);
   if (window)
      moveWindowToFront ((Window *)window);
}

void Screen::moveWindowToFront (Window * window)
{
}

void Screen::centerWindow (Window * window)
{
   if (window->size() == ivec2(0))
   {
      window->setSize (window->preferredSize (mNVGContext));
      window->performLayout (mNVGContext);
   }
   window->setPosition ((mSize - window->size()) / 2);
}

void Screen::disposeWindow (Window * window)
{
   if (std::find (mFocusPath.begin(), mFocusPath.end(), window) != mFocusPath.end())
      mFocusPath.clear();
   if (mDragWidget == window)
      mDragWidget = nullptr;
   removeChild (window);
}

bool Screen::resizeCallbackEvent (int width, int height)
{
   auto end = std::chrono::system_clock::now();
   mLastInteraction = end - start;
   try
   {
      return resizeEvent (width, height);
   }
   catch (const std::exception & e)
   {
      std::cerr << "Caught exception in event handler: " << e.what()
                << std::endl;
      //abort();
   }
   return false;
}


NAMESPACE_END (nanogui)
