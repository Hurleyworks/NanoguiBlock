/*
    src/vscrollpanel.cpp -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

VScrollPanel::VScrollPanel (Widget * parent)
   : Widget (parent), mChildPreferredHeight (0), mScroll (0.0f) {}

void VScrollPanel::performLayout (NVGcontext * ctx)
{
   if (mChildren.empty())
      return;
   Widget * child = mChildren[0];
   mChildPreferredHeight = child->preferredSize (ctx).y;
   child->setPosition (ivec2 (0, 0));
   child->setSize (ivec2 (mSize.x - 12, mChildPreferredHeight));
}

ivec2 VScrollPanel::preferredSize (NVGcontext * ctx) const
{
   if (mChildren.empty())
      return ivec2(0);
   return mChildren[0]->preferredSize (ctx) + ivec2 (12, 0);
}

bool VScrollPanel::mouseDragEvent (const ivec2 &, const ivec2 & rel,
                                   int, int)
{
   if (mChildren.empty())
      return false;
   float scrollh = height() *
                   std::min (1.0f, height() / (float)mChildPreferredHeight);
   mScroll = std::max ((float) 0.0f, std::min ((float) 1.0f,
                       mScroll + rel.y / (float) (mSize.y - 8 - scrollh)));
   return true;
}

bool VScrollPanel::scrollEvent (const ivec2 & /* p */, const vec2 & rel)
{
   float scrollAmount = rel.y * (mSize.y / 20.0f);
   float scrollh = height() *
                   std::min (1.0f, height() / (float)mChildPreferredHeight);
   mScroll = std::max ((float) 0.0f, std::min ((float) 1.0f,
                       mScroll - scrollAmount / (float) (mSize.y - 8 - scrollh)));
   return true;
}

bool VScrollPanel::mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers)
{
   if (mChildren.empty())
      return false;
   int shift = (int) (mScroll * (mChildPreferredHeight - mSize.y));
   return mChildren[0]->mouseButtonEvent (p + ivec2 (0, shift), button, down, modifiers);
}

bool VScrollPanel::mouseMotionEvent (const ivec2 & p, const ivec2 & rel, int button, int modifiers)
{
   if (mChildren.empty())
      return false;
   int shift = (int) (mScroll * (mChildPreferredHeight - mSize.y));
   return mChildren[0]->mouseMotionEvent (p + ivec2 (0, shift), rel, button, modifiers);
}

void VScrollPanel::draw (NVGcontext * ctx)
{
   if (mChildren.empty())
      return;
   Widget * child = mChildren[0];
   mChildPreferredHeight = child->preferredSize (ctx).y;
   float scrollh = height() *
                   std::min (1.0f, height() / (float) mChildPreferredHeight);
   nvgSave (ctx);
   nvgTranslate (ctx, mPos.x, mPos.y);
   nvgScissor (ctx, 0, 0, mSize.x, mSize.y);
   nvgTranslate (ctx, 0, -mScroll * (mChildPreferredHeight - mSize.y));
   if (child->visible())
      child->draw (ctx);
   nvgRestore (ctx);
   NVGpaint paint = nvgBoxGradient (
                       ctx, mPos.x + mSize.x - 12 + 1, mPos.y + 4 + 1, 8,
                       mSize.y - 8, 3, 4, Colour (0, 32), Colour (0, 92));
   nvgBeginPath (ctx);
   nvgRoundedRect (ctx, mPos.x + mSize.x - 12, mPos.y + 4, 8,
                   mSize.y - 8, 3);
   nvgFillPaint (ctx, paint);
   nvgFill (ctx);
   paint = nvgBoxGradient (
              ctx, mPos.x + mSize.x - 12 - 1,
              mPos.y + 4 + (mSize.y - 8 - scrollh) * mScroll - 1, 8, scrollh,
              3, 4, Colour (220, 100), Colour (128, 100));
   nvgBeginPath (ctx);
   nvgRoundedRect (ctx, mPos.x + mSize.x - 12 + 1,
                   mPos.x + 4 + 1 + (mSize.y - 8 - scrollh) * mScroll, 8 - 2,
                   scrollh - 2, 2);
   nvgFillPaint (ctx, paint);
   nvgFill (ctx);
}

NAMESPACE_END (nanogui)
