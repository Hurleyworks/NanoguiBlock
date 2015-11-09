/*
    src/theme.cpp -- Storage class for basic theme-related properties

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


NAMESPACE_BEGIN (nanogui)

Theme::Theme (NVGcontext * ctx)
{
   mStandardFontSize                 = 16;
   mButtonFontSize                   = 20;
   mTextBoxFontSize                  = 20;
   mWindowCornerRadius               = 2;
   mWindowHeaderHeight               = 30;
   mWindowDropShadowSize             = 10;
   mButtonCornerRadius               = 2;
   mDropShadow                       = Colour (0, 128);
   mTransparent                      = Colour (0, 0);
   mBorderDark                       = Colour (29, 255);
   mBorderLight                      = Colour (92, 255);
   mBorderMedium                     = Colour (35, 255);
   mTextColor                        = Colour (255, 160);
   mDisabledTextColor                = Colour (255, 80);
   mTextColorShadow                  = Colour (0, 160);
   mIconColor                        = mTextColor;
   mButtonGradientTopFocused         = Colour (64, 255);
   mButtonGradientBotFocused         = Colour (48, 255);
   mButtonGradientTopUnfocused       = Colour (74, 255);
   mButtonGradientBotUnfocused       = Colour (58, 255);
   mButtonGradientTopPushed          = Colour (41, 255);
   mButtonGradientBotPushed          = Colour (29, 255);
   /* Window-related */
   mWindowFillUnfocused              = Colour (43, 230);
   mWindowFillFocused                = Colour (45, 230);
   mWindowTitleUnfocused             = Colour (220, 160);
   mWindowTitleFocused               = Colour (255, 190);
   mWindowHeaderGradientTop          = mButtonGradientTopUnfocused;
   mWindowHeaderGradientBot          = mButtonGradientBotUnfocused;
   mWindowHeaderSepTop               = mBorderLight;
   mWindowHeaderSepBot               = mBorderDark;
   mWindowPopup                      = Colour (50, 255);
   mWindowPopupTransparent           = Colour (50, 0);
   mFontNormal = nvgCreateFontMem (ctx, "sans", roboto_regular_ttf,
                                   roboto_regular_ttf_size, 0);
   mFontBold = nvgCreateFontMem (ctx, "sans-bold", roboto_bold_ttf,
                                 roboto_bold_ttf_size, 0);
   mFontIcons = nvgCreateFontMem (ctx, "icons", entypo_ttf,
                                  entypo_ttf_size, 0);
   if (mFontNormal == -1 || mFontBold == -1 || mFontIcons == -1)
      throw std::runtime_error ("Could not load fonts!");
}

NAMESPACE_END (nanogui)
