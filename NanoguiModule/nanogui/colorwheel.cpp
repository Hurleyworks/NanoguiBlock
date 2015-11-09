/*
    src/colorwheel.cpp -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

ColorWheel::ColorWheel(Widget *parent, const Colour& rgb)
	: Widget(parent), mDragRegion(None) {
	setColor(rgb);
}

ivec2 ColorWheel::preferredSize(NVGcontext *) const {
	return{ 100, 100. };
}

void ColorWheel::draw(NVGcontext *ctx) {
	Widget::draw(ctx);

	if (!mVisible)
		return;

	float x = mPos.x,
		y = mPos.y,
		w = mSize.x,
		h = mSize.y;

	NVGcontext* vg = ctx;

	int i;
	float r0, r1, ax, ay, bx, by, cx, cy, aeps, r;
	float hue = mHue;
	NVGpaint paint;

	nvgSave(vg);

	cx = x + w*0.5f;
	cy = y + h*0.5f;
	r1 = (w < h ? w : h) * 0.5f - 5.0f;
	r0 = r1 * .75f;

	aeps = 0.5f / r1;   // half a pixel arc length in radians (2pi cancels out).

	for (i = 0; i < 6; i++) {
		float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
		float a1 = (float)(i + 1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
		nvgBeginPath(vg);
		nvgArc(vg, cx, cy, r0, a0, a1, NVG_CW);
		nvgArc(vg, cx, cy, r1, a1, a0, NVG_CCW);
		nvgClosePath(vg);
		ax = cx + cosf(a0) * (r0 + r1)*0.5f;
		ay = cy + sinf(a0) * (r0 + r1)*0.5f;
		bx = cx + cosf(a1) * (r0 + r1)*0.5f;
		by = cy + sinf(a1) * (r0 + r1)*0.5f;
		paint = nvgLinearGradient(vg, ax, ay, bx, by,
			nvgHSLA(a0 / (NVG_PI * 2), 1.0f, 0.55f, 255),
			nvgHSLA(a1 / (NVG_PI * 2), 1.0f, 0.55f, 255));
		nvgFillPaint(vg, paint);
		nvgFill(vg);
	}

	nvgBeginPath(vg);
	nvgCircle(vg, cx, cy, r0 - 0.5f);
	nvgCircle(vg, cx, cy, r1 + 0.5f);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
	nvgStrokeWidth(vg, 1.0f);
	nvgStroke(vg);

	// Selector
	nvgSave(vg);
	nvgTranslate(vg, cx, cy);
	nvgRotate(vg, hue*NVG_PI * 2);

	// Marker on
	float u = std::max(r1 / 50, 1.5f);
	u = std::min(u, 4.f);
	nvgStrokeWidth(vg, u);
	nvgBeginPath(vg);
	nvgRect(vg, r0 - 1, -2 * u, r1 - r0 + 2, 4 * u);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
	nvgStroke(vg);

	paint = nvgBoxGradient(vg, r0 - 3, -5, r1 - r0 + 6, 10, 2, 4, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, r0 - 2 - 10, -4 - 10, r1 - r0 + 4 + 20, 8 + 20);
	nvgRect(vg, r0 - 2, -4, r1 - r0 + 4, 8);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, paint);
	nvgFill(vg);

	// Center triangle
	r = r0 - 6;
	ax = cosf(120.0f / 180.0f*NVG_PI) * r;
	ay = sinf(120.0f / 180.0f*NVG_PI) * r;
	bx = cosf(-120.0f / 180.0f*NVG_PI) * r;
	by = sinf(-120.0f / 180.0f*NVG_PI) * r;
	nvgBeginPath(vg);
	nvgMoveTo(vg, r, 0);
	nvgLineTo(vg, ax, ay);
	nvgLineTo(vg, bx, by);
	nvgClosePath(vg);
	paint = nvgLinearGradient(vg, r, 0, ax, ay, nvgHSLA(hue, 1.0f, 0.5f, 255),
		nvgRGBA(255, 255, 255, 255));
	nvgFillPaint(vg, paint);
	nvgFill(vg);
	paint = nvgLinearGradient(vg, (r + ax) * 0.5f, (0 + ay) * 0.5f, bx, by,
		nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 255));
	nvgFillPaint(vg, paint);
	nvgFill(vg);
	nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
	nvgStroke(vg);

	// Select circle on triangle
	float sx = r*(1 - mWhite - mBlack) + ax*mWhite + bx*mBlack;
	float sy = ay*mWhite + by*mBlack;

	nvgStrokeWidth(vg, u);
	nvgBeginPath(vg);
	nvgCircle(vg, sx, sy, 2 * u);
	nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
	nvgStroke(vg);

	nvgRestore(vg);

	nvgRestore(vg);
}

bool ColorWheel::mouseButtonEvent(const ivec2 &p, int button, bool down,
	int modifiers) {
	Widget::mouseButtonEvent(p, button, down, modifiers);
	if (!mEnabled || button != MOUSE_BUTTON_LEFT)
		return false;

	if (down) {
		mDragRegion = adjustPosition(p);
		return mDragRegion != None;
	}
	else {
		mDragRegion = None;
		return true;
	}
}

bool ColorWheel::mouseDragEvent(const ivec2 &p, const ivec2 &,
	int, int) {
	return adjustPosition(p, mDragRegion) != None;
}

ColorWheel::Region ColorWheel::adjustPosition(const ivec2 &p, Region consideredRegions)
{

	float x = p.x - mPos.x, y = p.y - mPos.y, w = mSize.x, h = mSize.y;

	float cx = w*0.5f;
	float cy = h*0.5f;
	float r1 = (w < h ? w : h) * 0.5f - 5.0f;
	float r0 = r1 * .75f;

	x -= cx;
	y -= cy;

	float mr = glm::sqrt(x*x + y*y);

	if ((consideredRegions & OuterCircle) &&
		((mr >= r0 && mr <= r1) || (consideredRegions == OuterCircle))) {
		if (!(consideredRegions & OuterCircle))
			return None;
		mHue = std::atan(y / x);
		if (x < 0)
			mHue += NVG_PI;
		mHue /= 2 * NVG_PI;

		if (mCallback)
			mCallback(color());

		return OuterCircle;
	}

	float r = r0 - 6;

	float hueAngle = float(NVG_PI * 2) * mHue;
	const float angle = float(NVG_PI * 120 / 180);
	glm::vec2 a(r * glm::cos(hueAngle + angle), r * glm::sin(hueAngle + angle));
	glm::vec2 b(r * glm::cos(hueAngle - angle), r * glm::sin(hueAngle - angle));
	glm::vec2 c(r * glm::cos(hueAngle), r * glm::sin(hueAngle));

	vec3 bary = calcBarycentric(vec2(x, y), a, b, c);

	float l0 = bary[0], l1 = bary[1], l2 = bary[2];
	bool triangleTest = l0 >= 0 && l0 <= 1.f && l1 >= 0.f && l1 <= 1.f && l2 >= 0.f && l2 <= 1.f;

	if ((consideredRegions & InnerTriangle) &&
		(triangleTest || consideredRegions == InnerTriangle)) {
		if (!(consideredRegions & InnerTriangle))
			return None;
		l0 = std::min(std::max(0.f, l0), 1.f);
		l1 = std::min(std::max(0.f, l1), 1.f);
		l2 = std::min(std::max(0.f, l2), 1.f);
		float sum = l0 + l1 + l2;
		l0 /= sum;
		l1 /= sum;
		mWhite = l0;
		mBlack = l1;
		if (mCallback)
			mCallback(color());
		return InnerTriangle;
	}
	return None;
}

vec3 ColorWheel::calcBarycentric(const vec2 &pt, const vec2 &a, const vec2 &b, const vec2 &c) const
{
	vec3 result;

	vec2 v0 = b - a, v1 = c - a, v2 = pt - a;

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	result.y = (d11 * d20 - d01 * d21) / denom;
	result.z = (d00 * d21 - d01 * d20) / denom;
	result.x = 1.0f - result.y - result.z;

	return result;
}

vec3 ColorWheel::calcBarycentric(const vec3 & pt, const vec3 & a, const vec3 & b, const vec3 & c) const
{
	vec3 result;

	vec3 v0 = b - a, v1 = c - a, v2 = pt - a;

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	result.y = (d11 * d20 - d01 * d21) / denom;
	result.z = (d00 * d21 - d01 * d20) / denom;
	result.x = 1.0f - result.y - result.z;

	return result;
}

Colour ColorWheel::hue2rgb(float h) const {
	float s = 1., v = 1.;

	if (h < 0) h += 1;

	int i = int(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	float r = 0, g = 0, b = 0;
	switch (i % 6) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	return{ r, g, b, 1.f };
}

Colour ColorWheel::color() const {
	Colour rgb = hue2rgb(mHue);
	Colour black{ 0.f, 0.f, 0.f, 1.f };
	Colour white{ 1.f, 1.f, 1.f, 1.f };
	return rgb * (1 - mWhite - mBlack) + black * mBlack + white * mWhite;
}

void ColorWheel::setColor(const Colour &rgb)
{
	float r = rgb[0], g = rgb[1], b = rgb[2];

	float max = std::max({ r, g, b });
	float min = std::min({ r, g, b });
	float l = (max + min) / 2;

	if (max == min) {
		mHue = 0.;
		mBlack = 1. - l;
		mWhite = l;
	}
	else {
		float d = max - min, h;
		/* float s = l > 0.5 ? d / (2 - max - min) : d / (max + min); */
		if (max == r)
			h = (g - b) / d + (g < b ? 6 : 0);
		else if (max == g)
			h = (b - r) / d + 2;
		else
			h = (r - g) / d + 4;
		h /= 6;

		mHue = h;

		vec3 pt(rgb.r, rgb.g, rgb.b);
		Colour c = hue2rgb(mHue);
		vec3 a(c.r, c.g, c.b);
		vec3 b(0, 0, 0);
		vec3 w(1, 1, 1);

		vec3 bary = calcBarycentric(pt, a, w, b);

		/*	Eigen::Matrix<float, 4, 3> M;
		M.topLeftCorner<3, 1>() = hue2rgb(h).head<3>();
		M(3, 0) = 1.;
		M.col(1) = vec4{ 0., 0., 0., 1. };
		M.col(2) = vec4{ 1., 1., 1., 1. };

		vec4 rgb4{ rgb[0], rgb[1], rgb[2], 1. };
		vec3 bary = M.colPivHouseholderQr().solve(rgb4); */

		mBlack = bary[1];
		mWhite = bary[2];
	}
}

NAMESPACE_END(nanogui)


