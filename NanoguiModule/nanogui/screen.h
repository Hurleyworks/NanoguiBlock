/*
   nanogui/screen.h -- Top-level widget and interface between NanoGUI and GLFW

   A significant redesign of this code was contributed by Christian Schueller.

   NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
   The widget drawing code is based on the NanoVG demo application
   by Mikko Mononen.

   All rights reserved. Use of this source code is governed by a
   BSD-style license that can be found in the LICENSE.txt file.
*/

// This is a modified version of Nanogui!

#pragma once


NAMESPACE_BEGIN (nanogui)

class Screen : public Widget
{
      friend class Widget;
      friend class Window;

   public:
      Screen();
      virtual ~Screen();

      virtual void drawWidgets();
      bool cursorPosCallbackEvent (double x, double y);
      bool mouseButtonCallbackEvent (int button, int action, int modifiers);
      bool resizeCallbackEvent (int width, int height);
      bool keyCallbackEvent (int key, int scancode, int action, int mods);
      bool charCallbackEvent (unsigned int codepoint);

      /// Window resize event handler
      virtual bool resizeEvent (int /* width */, int /* height */)
      {
         return false;
      }

      /* Internal helper functions */
      void updateFocus (Widget * widget);
      void moveWindowToFront (Window * window);
      void centerWindow (Window * window);
      void disposeWindow (Window * window);

      NVGcontext * getContext()
      {
         return mNVGContext;
      }

      std::chrono::duration<double> elapsedTime()
      {
         // time elapsed since screen was initialized.
         auto end = std::chrono::system_clock::now();
         return  end - start;
      }

      /// Default keyboard event handler
      virtual bool keyboardEvent (int key, int scancode, int action, int modifiers);

      /// Text input event handler: codepoint is native endian UTF-32 format
      virtual bool keyboardCharacterEvent (unsigned int codepoint);

   protected:
      NVGcontext * mNVGContext = nullptr;
      bool mDragActive = false;
      Widget * mDragWidget = nullptr;
      int mMouseState = 0;
      int mModifiers = 0;
      std::vector<Widget *> mFocusPath;

      std::chrono::time_point<std::chrono::system_clock> start;
      std::chrono::duration<double> mLastInteraction;

      ivec2 mMousePos;

}; // end class Screen

NAMESPACE_END (nanogui)