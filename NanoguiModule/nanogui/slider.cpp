/*
    nanogui/slider.cpp -- Fractional slider widget with mouse control

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

Slider::Slider (Widget * parent)
   : Widget (parent), mValue (0.0f), mHighlightedRange (std::make_pair (0.f, 0.f))
{
   mHighlightColor = Colour (255, 80, 80, 70);
}

ivec2 Slider::preferredSize (NVGcontext *) const
{
   return ivec2 (70, 12);
}

bool Slider::mouseDragEvent (const ivec2 & p, const ivec2 & /* rel */,
                             int /* button */, int /* modifiers */)
{
   if (!mEnabled)
      return false;
   mValue = std::min (std::max ((p.x - mPos.x) / (float) mSize.x, (float) 0.0f), (float) 1.0f);
   if (mCallback)
      mCallback (mValue);
   return true;
}

bool Slider::mouseButtonEvent (const ivec2 & p, int /* button */, bool down, int /* modifiers */)
{
   if (!mEnabled)
      return false;
   mValue = std::min (std::max ((p.x - mPos.x) / (float) mSize.x, (float) 0.0f), (float) 1.0f);
   if (mCallback)
      mCallback (mValue);
   if (mFinalCallback && !down)
      mFinalCallback (mValue);
   return true;
}

void Slider::draw (NVGcontext * ctx)
{
	vec2 center = vec2(mPos) + vec2(mSize) * 0.5f;
	vec2 knobPos(mPos.x + mValue * mSize.x, center.y + 0.5f);
	float kr = (int)(mSize.y * 0.5f);
	NVGpaint bg = nvgBoxGradient(ctx,
		mPos.x, center.y - 3 + 1, mSize.x, 6, 3, 3, Colour(0, mEnabled ? 32 : 10), Colour(0, mEnabled ? 128 : 210));
	nvgBeginPath(ctx);
	nvgRoundedRect(ctx, mPos.x, center.y - 3 + 1, mSize.x, 6, 2);
	nvgFillPaint(ctx, bg);
	nvgFill(ctx);
	if (mHighlightedRange.second != mHighlightedRange.first)
	{
		nvgBeginPath(ctx);
		nvgRoundedRect(ctx, mPos.x + mHighlightedRange.first * mSize.x, center.y - 3 + 1, mSize.x * (mHighlightedRange.second - mHighlightedRange.first), 6, 2);
		nvgFillColor(ctx, mHighlightColor);
		nvgFill(ctx);
	}
	NVGpaint knobShadow = nvgRadialGradient(ctx,
		knobPos.x, knobPos.y, kr - 3, kr + 3, Colour(0, 64), mTheme->mTransparent);
	nvgBeginPath(ctx);
	nvgRect(ctx, knobPos.x - kr - 5, knobPos.y - kr - 5, kr * 2 + 10, kr * 2 + 10 + 3);
	nvgCircle(ctx, knobPos.x, knobPos.y, kr);
	nvgPathWinding(ctx, NVG_HOLE);
	nvgFillPaint(ctx, knobShadow);
	nvgFill(ctx);
	NVGpaint knob = nvgLinearGradient(ctx,
		mPos.x, center.y - kr, mPos.x, center.y + kr,
		mTheme->mBorderLight, mTheme->mBorderMedium);
	NVGpaint knobReverse = nvgLinearGradient(ctx,
		mPos.x, center.y - kr, mPos.x, center.y + kr,
		mTheme->mBorderMedium,
		mTheme->mBorderLight);
	nvgBeginPath(ctx);
	nvgCircle(ctx, knobPos.x, knobPos.y, kr);
	nvgStrokeColor(ctx, mTheme->mBorderDark);
	nvgFillPaint(ctx, knob);
	nvgStroke(ctx);
	nvgFill(ctx);
	nvgBeginPath(ctx);
	nvgCircle(ctx, knobPos.x, knobPos.y, kr / 2);
	nvgFillColor(ctx, Colour(150, mEnabled ? 255 : 100));
	nvgStrokePaint(ctx, knobReverse);
	nvgStroke(ctx);
	nvgFill(ctx);
}

NAMESPACE_END (nanogui)
