/*
    nanogui/vscrollpanel.h -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

NAMESPACE_BEGIN (nanogui)

class  VScrollPanel : public Widget
{
   public:
      VScrollPanel (Widget * parent);

      virtual void performLayout (NVGcontext * ctx);
      virtual ivec2 preferredSize (NVGcontext * ctx) const;
      virtual bool mouseDragEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers);
      virtual bool scrollEvent (const ivec2 & p, const vec2 & rel);
      virtual bool mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers);
      virtual bool mouseMotionEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers);
      virtual void draw (NVGcontext * ctx);
   protected:
      int mChildPreferredHeight;
      float mScroll;
};

NAMESPACE_END (nanogui)
