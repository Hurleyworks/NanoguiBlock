/*
    nanogui/theme.h -- Storage class for basic theme-related properties

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once


NAMESPACE_BEGIN (nanogui)

class  Theme : public Object
{
   public:
      Theme (NVGcontext * ctx);

      /* Fonts */
      int mFontNormal;
      int mFontBold;
      int mFontIcons;

      /* Spacing-related parameters */
      int mStandardFontSize;
      int mButtonFontSize;
      int mTextBoxFontSize;
      int mWindowCornerRadius;
      int mWindowHeaderHeight;
      int mWindowDropShadowSize;
      int mButtonCornerRadius;

      /* Generic colors */
      Colour mDropShadow;
      Colour mTransparent;
      Colour mBorderDark;
      Colour mBorderLight;
      Colour mBorderMedium;
      Colour mTextColor;
      Colour mDisabledTextColor;
      Colour mTextColorShadow;
      Colour mIconColor;

      /* Button colors */
      Colour mButtonGradientTopFocused;
      Colour mButtonGradientBotFocused;
      Colour mButtonGradientTopUnfocused;
      Colour mButtonGradientBotUnfocused;
      Colour mButtonGradientTopPushed;
      Colour mButtonGradientBotPushed;

      /* Window colors */
      Colour mWindowFillUnfocused;
      Colour mWindowFillFocused;
      Colour mWindowTitleUnfocused;
      Colour mWindowTitleFocused;

      Colour mWindowHeaderGradientTop;
      Colour mWindowHeaderGradientBot;
      Colour mWindowHeaderSepTop;
      Colour mWindowHeaderSepBot;

      Colour mWindowPopup;
      Colour mWindowPopupTransparent;
   protected:
      virtual ~Theme() { };
};

NAMESPACE_END (nanogui)
