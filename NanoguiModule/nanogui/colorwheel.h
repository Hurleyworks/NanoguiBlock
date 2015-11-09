/*
    nanogui/colorwheel.h -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

NAMESPACE_BEGIN(nanogui)

class  ColorWheel : public Widget
{
public:
	ColorWheel(Widget *parent, const Colour& color = { 1.f, 0.f, 0.f, 1.f });

	/// Set the change callback
	std::function<void(const Colour &)> callback() const { return mCallback; }
	void setCallback(const std::function<void(const Colour &)> &callback) { mCallback = callback; }

	/// Get the current color
	Colour color() const;
	/// Set the current color
	void setColor(const Colour& color);

	virtual ivec2 preferredSize(NVGcontext *ctx) const;
	virtual void draw(NVGcontext *ctx);
	virtual bool mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers);
	virtual bool mouseDragEvent(const ivec2 &p, const ivec2 &rel, int button, int modifiers);

private:
	enum Region {
		None = 0,
		InnerTriangle = 1,
		OuterCircle = 2,
		Both = 3
	};

	Colour hue2rgb(float h) const;
	Region adjustPosition(const ivec2 &p, Region consideredRegions = Both);
	vec3 calcBarycentric(const vec2 &pt, const vec2 &a, const vec2 &b, const vec2 &c) const;
	vec3 calcBarycentric(const vec3 &pt, const vec3 &a, const vec3 &b, const vec3 &c) const;

protected:
	float mHue;
	float mWhite;
	float mBlack;
	Region mDragRegion;
	std::function<void(const Colour &)> mCallback;
};

NAMESPACE_END(nanogui)
